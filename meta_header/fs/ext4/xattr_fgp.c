/*
* linux/fs/ext4/xattr_fgp.c
* fgp namespace.
*
* Copyright (C) 2018 by Prasad Wanigasinghe
*  
*/

#include "ext4_jbd2.h"
#include "ext4.h"
#include "xattr.h"

static size_t ext4_xattr_fgp_list(struct inode *inode, char *list, size_t list_size, const char *name, size_t name_len)
{
	const size_t prefix_len = XATTR_FGP_PREFIX_LEN;
	const size_t total_len = prefix_len + name_len + 1;
	
	#if FPM
		if (!test_opt(inode->i_sb, XATTR_USER))
			return 0;
	#endif

	if (list && total_len <= list_size) {
		memcpy(list, XATTR_FGP_PREFIX, prefix_len);
		memcpy(list+prefix_len, name, name_len);
		list[prefix_len + name_len] = '\0'; 
	}
	return total_len;
}

static int ext4_xattr_fgp_get(struct inode *inode, const char *name, void *buffer, size_t size)
{
	if (strcmp(name, "") == 0)
		return -EINVAL;
	#if FPM
		if (!test_opt(inode->i_sb, XATTR_USER))
			return -EOPNOTSUPP;
		#endif
			return ext4_xattr_get(inode, EXT4_XATTR_INDEX_FGP, name, buffer, size);
}

static int ext4_xattr_fgp_set(struct inode *inode, const char *name,const void *value, size_t size, int flags)
{ 
	if (strcmp(name, "") == 0) 
		return -EINVAL;
	#if FPM
		if (!test_opt(inode->i_sb, XATTR_USER))
			return -EOPNOTSUPP;
	#endif
		return ext4_xattr_set(inode, EXT4_XATTR_INDEX_FGP, name, value, size, flags);
} 

struct xattr_handler ext4_xattr_fgp_handler = { 
		.prefix = XATTR_FGP_PREFIX, 
		.list = ext4_xattr_fgp_list, 
		.get = ext4_xattr_fgp_get, 
		.set = ext4_xattr_fgp_set, 
};
