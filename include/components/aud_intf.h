#pragma once

#include "aud_intf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief AUD INTF API
 * @defgroup bk_aud_intf_api API group
 * @{
 */

/**
 * @brief     Init the AUD INTF driver
 *
 * This API init the resoure common:
 *   - Creat AUD INTF task
 *   - Configure default work mode
 *   - Register read mic data and write speaker data callback
 *
 * This API should be called before any other AUD INTF APIs.
 *
 * @param setup audio interface driver init configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_drv_init(aud_intf_drv_setup_t *setup);

/**
 * @brief     Deinit the AUD INTF driver
 *
 * This API deinit the resoure common:
 *   - Delete AUD INTF task
 *   - Reset default work mode
 *   - Unregister read mic data and write speaker data callback
 *
 * This API should be called when audio is not needed.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_drv_deinit(void);

/**
 * @brief     Set the AUD INTF work mode
 *
 * @param work_mode audio interface work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_mode(aud_intf_work_mode_t work_mode);

/**
 * @brief     Set the mic gain
 *
 * @param value the gain value range:0x00 ~ 0x3f
 *
 * This API should be called when onboard mic has been initialized.
 * And this API should not be called when UAC mic has been used.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_mic_gain(uint8_t value);

/**
 * @brief     Set the mic gain
 *
 * @param value the gain value range:0x00 ~ 0x3f
 *
 * This API should be called when speaker has been initialized
 * And this API should not be called when UAC speaker has been used.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_spk_gain(uint8_t value);

/**
 * @brief     Set the aec parameter
 *
 * @param aec_para the parameter
 * @param value the parameter value
 *
 * This API should be called when voice has been initialized and AEC is enable.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_aec_para(aud_intf_voc_aec_para_t aec_para, uint32_t value);

/**
 * @brief     Get the aec parameter
 *
 * This API should be called when voice has been initialized and AEC is enable.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_get_aec_para(void);

/*************************************** mic api *********************************************/
/**
 * @brief     Init the AUD INTF mic
 *
 * This API init the resoure common:
 *   - Init AUD adc driver
 *   - Configure AUD adc paramter
 *   - Configure DMA to carry adc data
 *   - Configure UAC mic
 *
 * This API should be called after bk_aud_intf_drv_init.
 *
 * @param mic_setup audio adc setup configuration
 *
 * Usage example:
 *
 *     audio_tras_setup_t aud_tras_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
 *     aud_intf_mic_setup_t aud_mic_setup = DEFAULT_AUD_INTF_MIC_SETUP_CONFIG();
 *
 *     aud_intf_drv_setup.work_mode = AUD_INTF_WORK_MODE_NULL;
 *     aud_intf_drv_setup.aud_intf_tx_mic_data = demo_udp_voice_send_packet;
 *     bk_aud_intf_drv_init(&aud_intf_drv_setup);
 *
 *     aud_work_mode = AUD_INTF_WORK_MODE_GENERAL;
 *     bk_aud_intf_set_mode(aud_work_mode);
 *
 *     bk_aud_intf_mic_init(&aud_intf_mic_setup);
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_mic_init(aud_intf_mic_setup_t *mic_setup);

/**
 * @brief     Deinit the AUD INTF mic
 *
 * This API deinit the resoure common:
 *   - Deinit AUD adc driver
 *   - Reset AUD adc paramter
 *   - Close DMA
 *   - Close UAC mic
 *
 * This API should be called after bk_aud_intf_mic_stop.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_aud_intf_mic_deinit(void);

/**
 * @brief     Start the AUD INTF mic work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_mic_start(void);

/**
 * @brief     Pause the AUD INTF mic work
 *
 * This API should be called after bk_aud_intf_mic_init.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_mic_pause(void);

/**
 * @brief     Stop the AUD INTF mic work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_mic_stop(void);

/**
 * @brief     Set the AUD INTF mic channel
 *
 * This API should be called after bk_aud_intf_mic_init.
 * This API only support onboard mic, and not support UAC mic.
 *
 * @param mic_chl audio interface mic channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_mic_chl(aud_intf_mic_chl_t mic_chl);

/**
 * @brief     Get the AUD INTF mic channel
 *
 * This API should be called after bk_aud_intf_mic_init.
 * This API only support onboard mic, and not support UAC mic.
 *
 * @param mic_chl save audio interface mic channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_get_mic_chl(aud_intf_mic_chl_t *mic_chl);

/**
 * @brief     Set the AUD INTF mic sample rate
 *
 * This API should be called after bk_aud_intf_mic_init.
 * This API only support onboard mic, and not support UAC mic.
 *
 * @param samp_rate audio interface mic sample rate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_mic_samp_rate(aud_adc_samp_rate_t samp_rate);

/**
 * @brief     Get the AUD INTF mic sample rate
 *
 * This API should be called after bk_aud_intf_mic_init.
 * This API only support onboard mic, and not support UAC mic.
 *
 * @param samp_rate save audio interface mic sample rate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_get_mic_samp_rate(aud_adc_samp_rate_t *samp_rate);


/*************************************** speaker api *********************************************/
/**
 * @brief     Init the AUD INTF speaker
 *
 * This API init the resoure common:
 *   - Init AUD dac driver
 *   - Configure AUD dac paramter
 *   - Configure DMA to carry dac data
 *   - Configure UAC speaker
 *
 * This API should be called after bk_aud_intf_drv_init.
 *
 * @param spk_setup audio speaker setup configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 * Usage example:
 *
 *     audio_tras_setup_t aud_tras_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
 *     aud_intf_spk_setup_t aud_spk_setup = DEFAULT_AUD_INTF_SPK_SETUP_CONFIG();
 *
 *     aud_intf_drv_setup.aud_intf_rx_spk_data = aud_read_pcm_data_from_sd;
 *     bk_aud_intf_drv_init(&aud_intf_drv_setup);
 *
 *     aud_work_mode = AUD_INTF_WORK_MODE_GENERAL;
 *     bk_aud_intf_set_mode(aud_work_mode);
 *
 *     aud_intf_spk_setup.frame_size = 640;
 *     bk_aud_intf_spk_init(&aud_intf_spk_setup);
 *
 */
bk_err_t bk_aud_intf_spk_init(aud_intf_spk_setup_t *spk_setup);

/**
 * @brief     Deinit the AUD INTF speaker
 *
 * This API deinit the resoure common:
 *   - Deinit AUD dac driver
 *   - Reset AUD dac paramter
 *   - Close DMA
 *   - Close UAC speaker
 *
 * This API should be called after bk_aud_intf_spk_stop.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_spk_deinit(void);

/**
 * @brief     Start the AUD INTF speaker work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_spk_start(void);

/**
 * @brief     Pause the AUD INTF speaker work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_spk_pause(void);

/**
 * @brief     Stop the AUD INTF speaker work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_spk_stop(void);

/**
 * @brief     Set the AUD INTF speaker channel
 *
 * @param spk_chl audio interface speaker channel
 *
 * This API should be called after bk_aud_intf_spk_init.
 * This API only support onboard speaker, and not support UAC speaker.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_spk_chl(aud_intf_spk_chl_t spk_chl);

/**
 * @brief     Get the AUD INTF speaker channel
 *
 * @param spk_chl save audio interface speaker channel
 *
 * This API should be called after bk_aud_intf_spk_init.
 * This API only support onboard speaker, and not support UAC speaker.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_get_spk_chl(aud_intf_mic_chl_t *spk_chl);

/**
 * @brief     Set the AUD INTF speaker sample rate
 *
 * This API should be called after bk_aud_intf_spk_init.
 * This API only support onboard speaker, and not support UAC speaker.
 *
 * @param samp_rate audio interface speaker sample rate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_spk_samp_rate(aud_dac_samp_rate_t samp_rate);

/**
 * @brief     Get the AUD INTF speaker sample rate
 *
 * This API should be called after bk_aud_intf_spk_init.
 * This API only support onboard speaker, and not support UAC speaker.
 *
 * @param samp_rate save audio interface speaker sample rate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_get_spk_samp_rate(aud_dac_samp_rate_t *samp_rate);


/*************************************** voice api *********************************************/
/**
 * @brief     Init the AUD INTF voice
 *
 * This API init the resoure common:
 *   - Init AUD voice transfer driver
 *   - Configure AUD dac and adc paramter
 *   - Configure DMA to carry adc and dac data
 *   - Configure UAC mic and speaker
 *
 * This API should be called after bk_aud_intf_drv_init.
 *
 * @param voc_setup audio voice setup configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 * Usage example:
 *
 *     audio_tras_setup_t aud_tras_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
 *     aud_intf_voc_setup_t aud_voc_setup = DEFAULT_AUD_INTF_VOC_SETUP_CONFIG();
 *
 *     aud_intf_drv_setup.aud_intf_tx_mic_data = demo_udp_voice_send_packet;
 *     bk_aud_intf_drv_init(&aud_intf_drv_setup);
 *
 *     aud_work_mode = AUD_INTF_WORK_MODE_VOICE;
 *     bk_aud_intf_set_mode(aud_work_mode);
 *
 *     bk_aud_intf_voc_init(aud_voc_setup);
 *
 */
bk_err_t bk_aud_intf_voc_init(aud_intf_voc_setup_t setup);

/**
 * @brief     Deinit the AUD INTF voice
 *
 * This API deinit the resoure common:
 *   - Deinit AUD voice transfer driver
 *   - Reset AUD adc and dac paramter
 *   - Close DMA
 *
 * This API should be called after bk_aud_intf_voice_stop.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_voc_deinit(void);

/**
 * @brief     Start the AUD INTF voice work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_voc_start(void);

/**
 * @brief     Stop the AUD INTF voice work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_voc_stop(void);

/**
 * @brief     Control mic
 *
 * @param mic_en
 *    - AUD_INTF_VOC_MIC_OPEN: enable mic
 *    - AUD_INTF_VOC_MIC_CLOSE: disable mic
 *
 * This API should be called in voice work mode.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_voc_mic_ctrl(aud_intf_voc_mic_ctrl_t mic_en);

/**
 * @brief     Control speaker
 *
 * @param spk_en
 *    - AUD_INTF_VOC_SPK_OPEN: enable speaker
 *    - AUD_INTF_VOC_SPK_CLOSE: disable speaker
 *
 * This API should be called in voice work mode.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_voc_spk_ctrl(aud_intf_voc_spk_ctrl_t spk_en);

/**
 * @brief     Control aec enable or disable
 *
 * @param aec_en
 *    - true: enable aec
 *    - false: disable aec
 *
 * This API should be called in voice work mode.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_voc_aec_ctrl(bool aec_en);

/**
 * @brief     Write the speaker data to audio dac
 *
 * @param dac_buff the address of speaker data written
 * @param size the data size (byte)
 *
 * This API should be called in general and voice work mode.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_write_spk_data(uint8_t *dac_buff, uint32_t size);

#if CONFIG_AUD_TRAS_DAC_DEBUG
/**
 * @brief     debug api of audio dac
 *
 * @param enable enable/disable audio dac debug
 *
 * This API should be called in voice work mode.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_set_voc_dac_debug(bool enable);
#endif

/**
 * @brief     register callback to dump tx data in voice work mode
 *
 * @param dump_callback callback
 *
 * This API should be called in voice work mode. And audio transfer driver will call the callback to save data.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_voc_tx_debug(aud_intf_dump_data_callback dump_callback);

/**
 * @brief     register callback to dump rx data in voice work mode
 *
 * @param dump_callback callback
 *
 * This API should be called in voice work mode. And audio transfer driver will call the callback to save data.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_voc_rx_debug(aud_intf_dump_data_callback dump_callback);

/**
 * @brief     register callback to dump aec data(include mic data, ref data, and out data) in voice work mode
 *
 * @param dump_callback callback
 *
 * This API should be called in voice work mode. And audio transfer driver will call the callback to save data.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_voc_aec_debug(aud_intf_dump_data_callback dump_callback);

#if CONFIG_VIDEO_AVI
/**
 * @brief     Start the AVI save work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_mic_save_start(void);

/**
 * @brief     Stop the AVI save work
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_intf_mic_save_stop(void);
#endif

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

