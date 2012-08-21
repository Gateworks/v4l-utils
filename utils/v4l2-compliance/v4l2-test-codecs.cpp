/*
    V4L2 API codec ioctl tests.

    Copyright (C) 2012  Hans Verkuil <hans.verkuil@cisco.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335  USA
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <assert.h>
#include "v4l2-compliance.h"

int testEncoder(struct node *node)
{
	struct v4l2_encoder_cmd cmd;
	int ret;

	memset(&cmd, 0xff, sizeof(cmd));
	memset(&cmd.raw, 0, sizeof(cmd.raw));
	ret = doioctl(node, VIDIOC_ENCODER_CMD, &cmd);
	if (ret == ENOTTY)
		return ret;
	fail_on_test(ret != EINVAL);
	ret = doioctl(node, VIDIOC_TRY_ENCODER_CMD, &cmd);
	fail_on_test(ret == ENOTTY);
	fail_on_test(ret != EINVAL);
	cmd.cmd = V4L2_ENC_CMD_STOP;
	cmd.flags = 0;
	ret = doioctl(node, VIDIOC_TRY_ENCODER_CMD, &cmd);
	fail_on_test(ret != 0);
	ret = doioctl(node, VIDIOC_ENCODER_CMD, &cmd);
	fail_on_test(ret != 0);
	cmd.cmd = V4L2_ENC_CMD_PAUSE;
	ret = doioctl(node, VIDIOC_ENCODER_CMD, &cmd);
	fail_on_test(ret != EPERM && ret != EINVAL);
	cmd.cmd = V4L2_ENC_CMD_RESUME;
	ret = doioctl(node, VIDIOC_ENCODER_CMD, &cmd);
	fail_on_test(ret != EPERM && ret != EINVAL);
	return 0;
}
