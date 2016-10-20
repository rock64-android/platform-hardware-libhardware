
#ifndef __RK_FH_H___
#define __RK_FH_H___

#define RK30_MAX_LCDC_SUPPORT	2
#define RK30_MAX_LAYER_SUPPORT	5
#define RK_MAX_FB_SUPPORT       5
#define RK_WIN_MAX_AREA		    4
#define RK_MAX_BUF_NUM       	11

#define u32 unsigned int
#define u8  unsigned char
#define u16 unsigned  short

/*ALPHA BLENDING MODE*/
enum alpha_mode {               /*  Fs       Fd */
	AB_USER_DEFINE     = 0x0,
	AB_CLEAR    	   = 0x1,/*  0          0*/
	AB_SRC      	   = 0x2,/*  1          0*/
	AB_DST    	       = 0x3,/*  0          1  */
	AB_SRC_OVER   	   = 0x4,/*  1   	    1-As''*/
	AB_DST_OVER    	   = 0x5,/*  1-Ad''   1*/
	AB_SRC_IN    	   = 0x6,
	AB_DST_IN    	   = 0x7,
	AB_SRC_OUT    	   = 0x8,
	AB_DST_OUT    	   = 0x9,
	AB_SRC_ATOP        = 0xa,
	AB_DST_ATOP    	   = 0xb,
	XOR                = 0xc,
	AB_SRC_OVER_GLOBAL = 0xd
}; /*alpha_blending_mode*/

struct rk_lcdc_post_cfg{
	u32 xpos;
	u32 ypos;
	u32 xsize;
	u32 ysize;
};

struct rk_fb_area_par {
	u8  data_format;        /*layer data fmt*/
    /**
     * fd_of_ion_buffer_to_display,
     * 只有本成员 大于 0, rk_fb_dev 才认为当前 area_par 是有效的.
     *
     * .DP : buffer_to_display : 当前 area 将显示的内容的 source buffer.
     * area 可能只显示 buffer 中的某一部分, 参见 'x_offset', 'y_offset', ...
     * .DP : memory_region_to_display : 将被显示出来的这部分, 记为 memory_region_to_display.
     */
	short ion_fd;
	unsigned int phy_addr;
	short acq_fence_fd;
    /**
     * 将 buffer_to_display 以 stride(x_vir) 为宽度的, 映射为一个二维区域,
     * memory_region_to_display 相对 buffer_to_display 起始位置(左上角) 的偏移, 像素为单位.
     */
	u16  x_offset;
	u16  y_offset;
    /**
     * memory_region_to_display 对应显示出的内容(.DP : content_displayed), 在 physical_display 上的 左上角位置, 像素为单位.
     */
	u16 xpos;		/*start point in panel  --->LCDC_WINx_DSP_ST*/
	u16 ypos;
    /**
     * content_displayed 在 physical_display 上的宽度和高度, 像素为单位.
     */
	u16 xsize;		/* display window width/height  -->LCDC_WINx_DSP_INFO*/
	u16 ysize;
    /**
     * memory_region_to_display 宽度和高度, 像素为单位.
     */
	u16 xact;		/*origin display window size -->LCDC_WINx_ACT_INFO*/
	u16 yact;
    /**
     * buffer_to_display 的宽度和高度, 像素为单位.
     */
	u16 xvir;		/*virtual width/height     -->LCDC_WINx_VIR*/
	u16 yvir;

	u8  fbdc_en;
	u8  fbdc_cor_en;
	u8  fbdc_data_format;

	u16 reserved0;
	u32 reserved1;
};

struct rk_fb_win_par {
	u8  win_id;
	u8  z_order;		/*win sel layer*/
	u8  alpha_mode;
	u16 g_alpha_val;
	u8  mirror_en;
	struct rk_fb_area_par area_par[RK_WIN_MAX_AREA];
	u32 reserved0;
};

/**
 * rk_fb_wins_cfg_t, 包含命令 rk_fb_dev 显示下一帧的所有数据.
 * 实例被作为 ioctl RK_FBIOSET_CONFIG_DONE 的参数, 传递到 rk_fb_dev,
 * 也将从 rk_fb_dev 中返回 release_fence_fds.
 *
 * rk_fb_dev 的层次结构如下 :
 *      rk_fb_dev, vop 硬件通常支持若干 win:
 *          rk_vop_win (win), 每个 win 可能支持配置若干 area
 *              rk_vop_are, area 是显示功能的最小单元.
 * 可对应到下面的数据类型定义中.
 */
struct rk_fb_win_cfg_data {
	u8  wait_fs;
    /**
     * 从 rk_fb_dev 返回的 retire_fence_fd.
     */
	short ret_fence_fd;
    /**
     * 从 rk_fb_dev 返回的 list of release_fence_fd for areas.
     */
	short rel_fence_fd[RK_MAX_BUF_NUM];
	struct  rk_fb_win_par win_par[RK30_MAX_LAYER_SUPPORT];
	struct  rk_lcdc_post_cfg post_cfg;
};

#endif /* __RK_FH_H___ */

