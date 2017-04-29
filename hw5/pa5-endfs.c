/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  Minor modifications and note by Andy Sayler (2012) <www.andysayler.com>

  Source: fuse-2.8.7.tar.gz examples directory
  http://sourceforge.net/projects/fuse/files/fuse-2.X/

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall `pkg-config fuse --cflags` fusexmp.c -o fusexmp `pkg-config fuse --libs`

  Note: This implementation is largely stateless and does not maintain
        open file handels between open and release calls (fi->fh).
        Instead, files are opened and closed as necessary inside read(), write(),
        etc calls. As such, the functions that rely on maintaining file handles are
        not implmented (fgetattr(), etc). Those seeking a more efficient and
        more complete implementation may wish to add fi->fh support to minimize
        open() and close() calls and support fh dependent functions.

*/

#define FUSE_USE_VERSION 28
#define HAVE_SETXATTR
#define PATH_MAX 4096
#define ENCRYPT 1
#define DECRYPT 0

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
/* Linux is missing ENOATTR error, using ENODATA instead */
#define ENOATTR ENODATA
#endif

/* Add namespace defintion for older kernels. Normally included in linux/xattr.h */
#ifndef XATTR_USER_PREFIX
#define XATTR_USER_PREFIX "user."
#define XATTR_USER_PREFIX_LEN (sizeof (XATTR_USER_PREFIX) - 1)
#define XATTR_NAME "pa5-encfs.encrypted"
#define XATTR_TRUE "true"
#define XATTR_FALSE "false"
#endif

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#include <stdlib.h>
#include "aes-crypt.h"
#endif

/* Struct to store filepath and passphrase */
struct xmp_state {
    char *mirrordir;
    char *phrase;
};
#define XMP_DATA ((struct xmp_state *) fuse_get_context()->private_data)

static void xmp_realpath(char rpath[PATH_MAX], const char *path)
{
    strcpy(rpath, XMP_DATA->mirrordir);
    strncat(rpath, path, PATH_MAX); // ridiculously long paths will break here
}

static void xmp_xattrset (char* rpath)
{
  char* tmpstr = NULL;
  /* Append necessary 'user.' namespace prefix to beginning of name string */
	tmpstr = malloc(strlen(XATTR_NAME) + XATTR_USER_PREFIX_LEN + 1);
	if(!tmpstr){
	    perror("malloc of 'tmpstr' error");
	    exit(EXIT_FAILURE);
	}
	strcpy(tmpstr, XATTR_USER_PREFIX);
	strcat(tmpstr, XATTR_NAME);
	/* Set attribute */
	if(setxattr(rpath, tmpstr, XATTR_TRUE, strlen(XATTR_TRUE), 0)){
	    perror("setxattr error");
	    fprintf(stderr, "path  = %s\n", rpath);
	    fprintf(stderr, "name  = %s\n", tmpstr);
	    fprintf(stderr, "value = %s\n", XATTR_TRUE);
	    fprintf(stderr, "size  = %zd\n", strlen(XATTR_TRUE));
	    exit(EXIT_FAILURE);
	}
	/* Cleanup */
	free(tmpstr);
}

static int xmp_chkxattr (char* rpath)
{
  char* tmpstr = NULL;
  int valsize = -1;

  /* Append necessary 'user.' namespace prefix to beginning of name string */
	tmpstr = malloc(strlen(XATTR_NAME) + XATTR_USER_PREFIX_LEN + 1);
	if(!tmpstr){
	    perror("malloc of 'tmpstr' error");
	    exit(EXIT_FAILURE);
	}
	strcpy(tmpstr, XATTR_USER_PREFIX);
	strcat(tmpstr, XATTR_NAME);
	/* Get attribute value size */
	valsize = getxattr(rpath, tmpstr, NULL, 0);

  return valsize;
}

static int xmp_encrypt (char template[], char* rpath)
{
  FILE* inFile = NULL;
  FILE* outFile = NULL;

  /* Open Files */
  inFile = fopen(template, "rb");
  if(!inFile){
    perror("infile fopen error");
    return EXIT_FAILURE;
  }
  outFile = fopen(rpath, "wb+");
  if(!outFile){
    perror("outfile fopen error");
    return EXIT_FAILURE;
  }

  if(!do_crypt(inFile, outFile, ENCRYPT, XMP_DATA->phrase)){
    fprintf(stderr, "do_crypt failed\n");
  }

  /* Cleanup */
  if(fclose(outFile)){
    perror("outFile fclose error\n");
  }
  if(fclose(inFile)){
    perror("inFile fclose error\n");
  }
  unlink(template);

  return 0;
}

static int xmp_decrypt(char template[], char* rpath)
{
  FILE* inFile = NULL;
  FILE* outFile = NULL;

  /* Open Files */
  inFile = fopen(rpath, "rb");
  if(!inFile){
    perror("infile fopen error");
    return EXIT_FAILURE;
  }
  outFile = fopen(template, "wb+");
  if(!outFile){
    perror("outfile fopen error");
    return EXIT_FAILURE;
  }

  if(!do_crypt(inFile, outFile, DECRYPT, XMP_DATA->phrase)){
    fprintf(stderr, "do_crypt failed\n");
  }

  /* Cleanup */
  if(fclose(outFile)){
    perror("outFile fclose error\n");
  }
  if(fclose(inFile)){
    perror("inFile fclose error\n");
  }

  return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res;

	res = lstat(rpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_access(const char *path, int mask)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res;

	res = access(rpath, mask);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res;

	res = readlink(rpath, buf, size - 1);
	if (res == -1)
		return -errno;

	buf[res] = '\0';
	return 0;
}


static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(rpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if (filler(buf, de->d_name, &st, 0))
			break;
	}

	closedir(dp);
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res;

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(mode)) {
		res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(rpath, mode);
	else
		res = mknod(rpath, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res;

	res = mkdir(rpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res;

	res = unlink(rpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res;

	res = rmdir(rpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_symlink(const char *from, const char *to)
{
	int res;

	res = symlink(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	int res;

	res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_link(const char *from, const char *to)
{
	int res;

	res = link(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res;

	res = chmod(rpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res;

	res = lchown(rpath, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res;

	res = truncate(rpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res;
	struct timeval tv[2];

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(rpath, tv);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res;

	res = open(rpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int fd;
	int res;
  char template[] = "/tmp/myfileXXXXXX";

  if(xmp_chkxattr(rpath) < 0){
    fd = open(rpath, O_RDONLY);
	   if (fd == -1)
		   return -errno;
  } else {
    fd = mkstemp(template);
    if(fd == -1){
      return -errno;
    }

    xmp_decrypt(template, rpath);
  }

	(void) fi;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
  unlink(template);
	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	//* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int fd;
	int res;
  char template[] = "/tmp/myfileXXXXXX";

	(void) fi;
  fd = mkstemp(template);
  if(fd == -1){
    return -errno;
  }

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

/* ENCRYPT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  xmp_encrypt(template, rpath);

/* SET ATTRIBUTE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  xmp_xattrset(rpath);

	return res;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res;

	res = statvfs(rpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) {
		/* Fixing path */
		char rpath[PATH_MAX];
		xmp_realpath(rpath, path);
    char template[] = "/tmp/myfileXXXXXX";

    (void) fi;

    int fd;
    fd = creat(template, mode);
    if(fd == -1){
      return -errno;
    }

    int res;
    res = creat(rpath, mode);
    if(res == -1)
	    return -errno;

    close(fd);
    close(res);

    /* ENCRYPT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    xmp_encrypt(template, rpath);

    /* SET ATTRIBUTE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    xmp_xattrset(rpath);

    return 0;
}


static int xmp_release(const char *path, struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	(void) path;
	(void) fi;
	return 0;
}

static int xmp_fsync(const char *path, int isdatasync,
		     struct fuse_file_info *fi)
{

	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	(void) path;
	(void) isdatasync;
	(void) fi;
	return 0;
}

#ifdef HAVE_SETXATTR
static int xmp_setxattr(const char *path, const char *name, const char *value,
			size_t size, int flags)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res = lsetxattr(rpath, name, value, size, flags);
	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_getxattr(const char *path, const char *name, char *value,
			size_t size)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res = lgetxattr(rpath, name, value, size);
	if (res == -1)
		return -errno;
	return res;
}

static int xmp_listxattr(const char *path, char *list, size_t size)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res = llistxattr(rpath, list, size);
	if (res == -1)
		return -errno;
	return res;
}

static int xmp_removexattr(const char *path, const char *name)
{
	/* Fixing path */
	char rpath[PATH_MAX];
  xmp_realpath(rpath, path);

	int res = lremovexattr(rpath, name);
	if (res == -1)
		return -errno;
	return 0;
}
#endif /* HAVE_SETXATTR */

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.access		= xmp_access,
	.readlink	= xmp_readlink,
	.readdir	= xmp_readdir,
	.mknod		= xmp_mknod,
	.mkdir		= xmp_mkdir,
	.symlink	= xmp_symlink,
	.unlink		= xmp_unlink,
	.rmdir		= xmp_rmdir,
	.rename		= xmp_rename,
	.link		= xmp_link,
	.chmod		= xmp_chmod,
	.chown		= xmp_chown,
	.truncate	= xmp_truncate,
	.utimens	= xmp_utimens,
	.open		= xmp_open,
	.read		= xmp_read,
	.write		= xmp_write,
	.statfs		= xmp_statfs,
	.create         = xmp_create,
	.release	= xmp_release,
	.fsync		= xmp_fsync,
#ifdef HAVE_SETXATTR
	.setxattr	= xmp_setxattr,
	.getxattr	= xmp_getxattr,
	.listxattr	= xmp_listxattr,
	.removexattr	= xmp_removexattr,
#endif
};

int main(int argc, char *argv[])
{
	umask(0);

	struct xmp_state *xmp_data;

	/* Check that right ammount of params are passed in */
	if ((argc < 4) || (argv[argc-2][0] == '-') || (argv[argc-1][0] == '-')){
		fprintf(stderr, "usage:  ./fusexmp passPhrase mirrorDir mountPoint\n");
		abort();
	}

	xmp_data = malloc(sizeof(struct xmp_state));
	if (xmp_data == NULL) {
		perror("Malloc error");
		abort();
	}

	xmp_data->mirrordir = realpath(argv[argc-2], NULL);
	xmp_data->phrase = argv[argc-3];
	argv[argc-3] = argv[argc-1];
	argv[argc-1] = NULL;
	argv[argc-2] = NULL;
	argc -= 2;

	// printf("%s\n", xmp_data->mirrordir);

	// return 0;
	return fuse_main(argc, argv, &xmp_oper, xmp_data);
}
