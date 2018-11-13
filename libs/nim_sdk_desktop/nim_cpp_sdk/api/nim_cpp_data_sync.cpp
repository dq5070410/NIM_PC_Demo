﻿/** @file nim_cpp_data_sync.cpp
  * @brief NIM SDK提供的数据同步相关接口
  * @copyright (c) 2015-2017, NetEase Inc. All rights reserved
  * @author towik, Oleg, Harrison
  * @date 2015/09/23
  */

#include "nim_cpp_data_sync.h"
#include "nim_sdk_util.h"
#include "nim_json_util.h"
#include "nim_cpp_win32_demo_helper.h"
#include "nim_string_util.h"


namespace nim
{
#ifdef NIM_SDK_DLL_IMPORT
typedef void(*nim_data_sync_reg_complete_cb)(nim_data_sync_cb_func cb, const void *user_data);
#else
#include "nim_data_sync.h"
#endif

static void CallbackSyncComplete(nim::NIMDataSyncType sync_type, nim::NIMDataSyncStatus status, const char *json_attachment, const void *user_data)
{
	if (user_data)
	{
		DataSync::DataSyncCallback* cb_pointer = (DataSync::DataSyncCallback*)user_data;
		if (*cb_pointer)
		{
			PostTaskToUIThread(std::bind((*cb_pointer), sync_type, status, PCharToString(json_attachment)));
// 			(*cb_pointer)(sync_type, status);
		}
	}
}

static DataSync::DataSyncCallback g_cb_data_sync_ = nullptr;
void DataSync::RegCompleteCb(const DataSyncCallback& cb)
{
	g_cb_data_sync_ = cb;
	return NIM_SDK_GET_FUNC(nim_data_sync_reg_complete_cb)(&CallbackSyncComplete, &g_cb_data_sync_);
}

void DataSync::UnregDataSyncCb()
{
	g_cb_data_sync_ = nullptr;
}



}