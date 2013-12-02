/*
 * Copyright (C) 2010, 2012 ARM Limited. All rights reserved.
 * 
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 * 
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <linux/fs.h>       /* file system operations */
#include <asm/uaccess.h>    /* user space access */

#include "mali_ukk.h"
#include "mali_osk.h"
#include "mali_kernel_common.h"
#include "mali_session.h"
#include "mali_ukk_wrappers.h"

int pp_start_job_wrapper_ggy_ggy(struct maliggy_session_data *session_data, _maliggy_uk_pp_start_job_s __user *uargs)
{
	_maliggy_osk_errcode_t err;
	int fence = -1;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	err = _maliggy_ukk_pp_start_job(session_data, uargs, &fence);
	if (_MALI_OSK_ERR_OK != err) return map_errcode_ggy_ggy(err);

#if defined(CONFIG_SYNC)
	if (0 != put_user(fence, &uargs->fence))
	{
		/* Since the job has started we can't return an error. */
	}
#endif /* CONFIG_SYNC */

	return 0;
}

int pp_get_number_of_cores_wrapper_ggy_ggy(struct maliggy_session_data *session_data, _maliggy_uk_get_pp_number_of_cores_s __user *uargs)
{
	_maliggy_uk_get_pp_number_of_cores_s kargs;
	_maliggy_osk_errcode_t err;

	MALI_CHECK_NON_NULL(uargs, -EINVAL);
	MALI_CHECK_NON_NULL(session_data, -EINVAL);

	kargs.ctx = session_data;

	err = _maliggy_ukk_get_pp_number_of_cores(&kargs);
	if (_MALI_OSK_ERR_OK != err)
	{
		return map_errcode_ggy_ggy(err);
	}

	kargs.ctx = NULL; /* prevent kernel address to be returned to user space */
	if (0 != copy_to_user(uargs, &kargs, sizeof(_maliggy_uk_get_pp_number_of_cores_s)))
	{
		return -EFAULT;
	}

	return 0;
}

int pp_get_core_version_wrapper_ggy_ggy(struct maliggy_session_data *session_data, _maliggy_uk_get_pp_core_version_s __user *uargs)
{
    _maliggy_uk_get_pp_core_version_s kargs;
    _maliggy_osk_errcode_t err;

    MALI_CHECK_NON_NULL(uargs, -EINVAL);
    MALI_CHECK_NON_NULL(session_data, -EINVAL);

    kargs.ctx = session_data;
    err = _maliggy_ukk_get_pp_core_version(&kargs);
    if (_MALI_OSK_ERR_OK != err) return map_errcode_ggy_ggy(err);

    if (0 != put_user(kargs.version, &uargs->version)) return -EFAULT;

    return 0;
}

int pp_disable_wb_wrapper_ggy_ggy_ggy(struct maliggy_session_data *session_data, _maliggy_uk_pp_disable_wb_s __user *uargs)
{
	_maliggy_uk_pp_disable_wb_s kargs;

    MALI_CHECK_NON_NULL(uargs, -EINVAL);
    MALI_CHECK_NON_NULL(session_data, -EINVAL);

    if (0 != copy_from_user(&kargs, uargs, sizeof(_maliggy_uk_pp_disable_wb_s))) return -EFAULT;

    kargs.ctx = session_data;
    _maliggy_ukk_pp_job_disable_wb(&kargs);

    return 0;
}
