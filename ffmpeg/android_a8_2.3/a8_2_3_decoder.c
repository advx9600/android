#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a8_2_3_decoder.h"

#define INPUT_BUFFER_SIZE   (204800)

int decoder_init(A8MfcParam* param) {
	void* pPhyStrmBuf;
	param->initialized = 0;
	param->mfcHandle = (_MFCLIB*) SsbSipMfcDecOpen();
	if (param->mfcHandle == NULL) {
		LOGE("SsbSipMfcDecOpen failed!");
		return -1;
	}
	param->mfcStrmBuf = SsbSipMfcDecGetInBuf(param->mfcHandle, &pPhyStrmBuf,
			INPUT_BUFFER_SIZE);
	if (param->mfcStrmBuf == NULL) {
		LOGE("SsbSipMfcDecGetInBuf failed!\n");
		return -1;
	}
	param->outputY = malloc(2048 * 2048);
	param->outputUV = malloc(2048 * 2048 / 2);
	param->swapBuf = malloc(2048*2048/2);

	if (param->outputY == NULL || param->outputUV == NULL || \
			param->swapBuf == NULL) {
		LOGE("malloc yuv data failed!");
		return -1;
	}
	return 0;
}
#if 0
static void savefile2(const char* filename,char* buf,int len)
{
  FILE* fd = NULL;
	fd=fopen(filename,"rb");
	if (fd == NULL) {
		fd = fopen(filename,"wb");
		if (fd == NULL){
			LOGE("fopen %s failed!",filename);
			return ;
		}
	}
  if (fwrite(buf,1,len,fd)<0){
    LOGE("fwrite file failed");
  }
	fclose(fd);
}
#endif
int decoder_exe(A8MfcParam* param, char* buf, int len) {
	int i,ret;
	SSBSIP_MFC_DEC_OUTBUF_STATUS output_status;
	int isSpspps =0;
	
	#if 1
	if (!param->initialized) {
		for (i=3;i<len && i<5;i++)
		{
			if ((buf[i-3]&0xff)==0 && (buf[i-2]&0xff)==0 && (buf[i-1]&0xff)==1  \
				&& (buf[i]&0xff)==0x67){
				isSpspps =1;
				break;
			}
		}
		if (!isSpspps){
			LOGW("decode 264 header not spspps data");
			return 0;
		}
	}
	#endif
	
	memcpy(param->mfcStrmBuf, buf, len);

	if (!param->initialized) {

		ret = SsbSipMfcDecInit(param->mfcHandle, param->codecType, len);
		
		if (ret != MFC_RET_OK) {
			LOGE("SsbSipMfcDecInit failed!\n");
			return -1;
		} else {
			param->initialized = 1;
			output_status = SsbSipMfcDecGetOutBuf(param->mfcHandle,
					&param->output_info);
			LOGV("out_status is 0x%x\n", output_status);
			LOGV(
					"width:%d,height:%d\n", param->output_info.img_width, param->output_info.img_height);
			param->outputU = param->outputUV;
			param->outputV = param->outputUV + param->output_info.img_width*param->output_info.img_height/4;
			return 0;
		}
	}

	ret = SsbSipMfcDecExe(param->mfcHandle, len);
	if (ret != MFC_RET_OK) {
		LOGE("SsbSipMfcDecExe failed!%d\n", ret);
		goto failed;
	}
//	LOGI("decode 264 %d success",len);
	memset(&param->output_info, 0, sizeof(SSBSIP_MFC_DEC_OUTPUT_INFO));
	if (SsbSipMfcDecGetOutBuf(param->mfcHandle, &param->output_info)
			!= MFC_GETOUTBUF_DISPLAY_DECODING) {
		LOGW(
				"SsbSipMfcDecGetOutBuf(mfcHandle, &output_info)!= MFC_GETOUTBUF_DISPLAY_DECODING");
		return 0;
	}

	#define USE_NEON 1
	#if USE_NEON
	tile_to_linear_64x32_4x2_neon(param->outputY, param->output_info.YVirAddr,
			param->output_info.img_width, param->output_info.img_height);
	tile_to_linear_64x32_4x2_uv_neon(param->outputUV,
			param->output_info.CVirAddr, param->output_info.img_width,
			param->output_info.img_height>>1);
	#else
	Y_tile_to_linear_4x2(param->outputY, param->output_info.YVirAddr,
			param->output_info.img_width, param->output_info.img_height);
	CbCr_tile_to_linear_4x2(param->outputUV,
			param->output_info.CVirAddr, param->output_info.img_width,
			param->output_info.img_height>>1);
	#endif

	return param->output_info.img_width * param->output_info.img_height * 3 / 2;

	failed: return -1;
}

void decoder_close(A8MfcParam* param) {
	if (param->mfcHandle != NULL)
		SsbSipMfcDecClose(param->mfcHandle);
	if (param->outputY != NULL)
		free(param->outputY);
	if (param->outputUV != NULL)
		free(param->outputUV);
	if (param->swapBuf != NULL)
		free(param->swapBuf);
}
