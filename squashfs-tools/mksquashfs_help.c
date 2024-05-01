/*
 * Squashfs
 *
 * Copyright (c) 2024
 * Phillip Lougher <phillip@squashfs.org.uk>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * mksquashfs_help.c
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "mksquashfs_help.h"
#include "compressor.h"

static char *options[] = {
	NULL, "-b", "-comp", "-noI", "-noId", "-noD", "-noF", "-noX", "-no-compression",
	NULL, "-tar", "-no-strip", "-tarstyle", "-cpiostyle", "-cpiostyle0",
	"-reproducible", "-not-reproducible", "-mkfs-time", "-all-time",
	"-root-time", "-root-mode", "-root-uid", "-root-gid", "-all-root",
	"-force-uid", "-force-gid", "-pseudo-override", "-no-exports",
	"-exports", "-no-sparse", "-no-tailends", "-tailends", "-no-fragments",
	"-no-duplicates", "-no-hardlinks", "-keep-as-directory", NULL, "-p",
	"-pd", "-pf", "-sort", "-wildcards", "-regex", "-max-depth",
	"-one-file-system", "-one-file-system-x", NULL, "-no-xattrs", "-xattrs",
	"-xattrs-exclude", "-xattrs-include", "-xattrs-add", NULL, "-version",
	"-exit-on-error", "-quiet", "-info", "-no-progress", "-progress",
	"-percentage", "-throttle", "-limit", "-processors", "-mem",
	"-mem-percent", "-mem-default", NULL, "-noappend", "-root-becomes",
	"-no-recovery", "-recovery-path", "-recover", NULL, "-action",
	"-log-action", "-true-action", "-false-action", "-action-file",
	"-log-action-file", "-true-action-file", "-false-action-file", NULL,
	"-default-mode", "-default-uid", "-default-gid", "-ignore-zeros", NULL,
	"-nopad", "-offset", "-o", NULL, "-fstime", "-always-use-fragments",
	"-root-owned", "-noInodeCompression", "-noIdTableCompression",
	"-noDataCompression", "-noFragmentCompression", "-noXattrCompression",
	"", "-help", "-h", "", "-Xhelp", NULL, NULL
};

static char *options_text[]={
	"Filesystem compression options:\n",
	"-b <block-size>\t\tset data block to <block-size>.  Default 128 Kbytes.\n\t\t\tOptionally a suffix of K, KB, Kbytes or M, MB, Mbytes\n\t\t\tcan be given to specify Kbytes or Mbytes respectively\n",
	"-comp <comp>\t\tselect <comp> compression\n\t\t\tCompressors available:\n\t\t\t\t" COMPRESSORS "\n",
	"-noI\t\t\tdo not compress inode table\n",
	"-noId\t\t\tdo not compress the uid/gid table (implied by -noI)\n",
	"-noD\t\t\tdo not compress data blocks\n",
	"-noF\t\t\tdo not compress fragment blocks\n",
	"-noX\t\t\tdo not compress extended attributes\n",
	"-no-compression\t\tdo not compress any of the data or metadata.  This is\n\t\t\tequivalent to specifying -noI -noD -noF and -noX\n",
	"\nFilesystem build options:\n",
	"-tar\t\t\tread uncompressed tar file from standard in (stdin)\n",
	"-no-strip\t\tact like tar, and do not strip leading directories\n\t\t\tfrom source files\n",
	"-tarstyle\t\talternative name for -no-strip\n",
	"-cpiostyle\t\tact like cpio, and read file pathnames from standard in\n\t\t\t(stdin)\n",
	"-cpiostyle0\t\tlike -cpiostyle, but filenames are null terminated.  Can\n\t\t\tbe used with find -print0 action\n",
	"-reproducible\t\tbuild filesystems that are reproducible" REP_STR "\n",
	"-not-reproducible\tbuild filesystems that are not reproducible" NOREP_STR "\n",
	"-mkfs-time <time>\tset filesystem creation timestamp to <time>. <time> can\n\t\t\tbe an unsigned 32-bit int indicating seconds since the\n\t\t\tepoch (1970-01-01) or a string value which is passed to\n\t\t\tthe \"date\" command to parse. Any string value which the\n\t\t\tdate command recognises can be used such as \"now\",\n\t\t\t\"last week\", or \"Wed Feb 15 21:02:39 GMT 2023\"\n",
	"-all-time <time>\tset all file timestamps to <time>. <time> can be an\n\t\t\tunsigned 32-bit int indicating seconds since the epoch\n\t\t\t(1970-01-01) or a string value which is passed to the\n\t\t\t\"date\" command to parse. Any string value which the date\n\t\t\tcommand recognises can be used such as \"now\", \"last\n\t\t\tweek\", or \"Wed Feb 15 21:02:39 GMT 2023\"\n",
	"-root-time <time>\tset root directory time to <time>. <time> can be an\n\t\t\tunsigned 32-bit int indicating seconds since the epoch\n\t\t\t(1970-01-01) or a string value which is passed to the\n\t\t\t\"date\" command to parse. Any string value which the date\n\t\t\tcommand recognises can be used such as \"now\", \"last\n\t\t\tweek\", or \"Wed Feb 15 21:02:39 GMT 2023\"\n",
	"-root-mode <mode>\tset root directory permissions to octal <mode>\n",
	"-root-uid <value>\tset root directory owner to specified <value>,\n\t\t\t<value> can be either an integer uid or user name\n",
	"-root-gid <value>\tset root directory group to specified <value>,\n\t\t\t<value> can be either an integer gid or group name\n",
	"-all-root\t\tmake all files owned by root\n",
	"-force-uid <value>\tset all file uids to specified <value>, <value> can be\n\t\t\teither an integer uid or user name\n",
	"-force-gid <value>\tset all file gids to specified <value>, <value> can be\n\t\t\teither an integer gid or group name\n",
	"-pseudo-override\tmake pseudo file uids and gids override -all-root,\n\t\t\t-force-uid and -force-gid options\n",
	"-no-exports\t\tdo not make filesystem exportable via NFS (-tar default)\n",
	"-exports\t\tmake filesystem exportable via NFS (default)\n",
	"-no-sparse\t\tdo not detect sparse files\n",
	"-no-tailends\t\tdo not pack tail ends into fragments (default)\n",
	"-tailends\t\tpack tail ends into fragments\n",
	"-no-fragments\t\tdo not use fragments\n",
	"-no-duplicates\t\tdo not perform duplicate checking\n",
	"-no-hardlinks\t\tdo not hardlink files, instead store duplicates\n",
	"-keep-as-directory\tif one source directory is specified, create a root\n\t\t\tdirectory containing that directory, rather than the\n\t\t\tcontents of the directory\n",
	"\nFilesystem filter options:\n",
	"-p <pseudo-definition>\tadd pseudo file definition.  The definition should\n\t\t\tbe quoted.  See section \"Pseudo file definition format\"\n\t\t\tlater for format details\n",
	"-pd <d mode uid gid>\tspecify a default pseudo directory which will be used in\n\t\t\tpseudo definitions if a directory in the pathname does\n\t\t\tnot exist.  This also allows pseudo definitions to be\n\t\t\tspecified without specifying all the directories in the\n\t\t\tpathname.  The definition should be quoted\n-pd <D time mode u g>\tas above, but also allow a timestamp to be specified\n",
	"-pf <pseudo-file>\tadd list of pseudo file definitions from <pseudo-file>,\n\t\t\tuse - for stdin.  Pseudo file definitions should not be\n\t\t\tquoted\n",
	"-sort <sort-file>\tsort files according to priorities in <sort-file>.  One\n\t\t\tfile or dir with priority per line.  Priority -32768 to\n\t\t\t32767, default priority 0\n-ef <exclude-file>\tlist of exclude dirs/files.  One per line\n",
	"-wildcards\t\tallow extended shell wildcards (globbing) to be used in\n\t\t\texclude dirs/files\n",
	"-regex\t\t\tallow POSIX regular expressions to be used in exclude\n\t\t\tdirs/files\n",
	"-max-depth <levels>\tdescend at most <levels> of directories when scanning\n\t\t\tfilesystem\n",
	"-one-file-system\tdo not cross filesystem boundaries.  If a directory\n\t\t\tcrosses the boundary, create an empty directory for\n\t\t\teach mount point.  If a file crosses the boundary\n\t\t\tignore it\n",
	"-one-file-system-x\tdo not cross filesystem boundaries. Like\n\t\t\t-one-file-system option except directories are also\n\t\t\tignored if they cross the boundary\n",
	"\nFilesystem extended attribute (xattrs) options:\n",
	"-no-xattrs\t\tdo not store extended attributes" NOXOPT_STR "\n",
	"-xattrs\t\t\tstore extended attributes" XOPT_STR "\n",
	"-xattrs-exclude <regex>\texclude any xattr names matching <regex>.  <regex> is a\n\t\t\tPOSIX regular expression, e.g. -xattrs-exclude '^user.'\n\t\t\texcludes xattrs from the user namespace\n",
	"-xattrs-include <regex>\tinclude any xattr names matching <regex>.  <regex> is a\n\t\t\tPOSIX regular expression, e.g. -xattrs-include '^user.'\n\t\t\tincludes xattrs from the user namespace\n",
	"-xattrs-add <name=val>\tadd the xattr <name> with <val> to files.  If an\n\t\t\tuser xattr it will be added to regular files and\n\t\t\tdirectories (see man 7 xattr).  Otherwise it will be\n\t\t\tadded to all files.  <val> by default will be treated as\n\t\t\tbinary (i.e. an uninterpreted byte sequence), but it can\n\t\t\tbe prefixed with 0s, where it will be treated as base64\n\t\t\tencoded, or prefixed with 0x, where val will be treated\n\t\t\tas hexidecimal.  Additionally it can be prefixed with\n\t\t\t0t where this encoding is similar to binary encoding,\n\t\t\texcept backslashes are specially treated, and a\n\t\t\tbackslash followed by 3 octal digits can be used to\n\t\t\tencode any ASCII character, which obviously can be used\n\t\t\tto encode control codes.  The option can be repeated\n"
"\t\t\tmultiple times to add multiple xattrs\n",
	"\nMksquashfs runtime options:\n",
	"-version\t\tprint version, licence and copyright message\n",
	"-exit-on-error\t\ttreat normally ignored errors as fatal\n",
	"-quiet\t\t\tno verbose output\n",
	"-info\t\t\tprint files written to filesystem\n",
	"-no-progress\t\tdo not display the progress bar\n",
	"-progress\t\tdisplay progress bar when using the -info option\n",
	"-percentage\t\tdisplay a percentage rather than the full progress bar.\n\t\t\tCan be used with dialog --gauge etc.\n",
	"-throttle <percentage>\tthrottle the I/O input rate by the given percentage.\n\t\t\tThis can be used to reduce the I/O and CPU consumption\n\t\t\tof Mksquashfs\n",
	"-limit <percentage>\tlimit the I/O input rate to the given percentage.\n\t\t\tThis can be used to reduce the I/O and CPU consumption\n\t\t\tof Mksquashfs (alternative to -throttle)\n",
	"-processors <number>\tuse <number> processors.  By default will use number of\n\t\t\tprocessors available\n",
	"-mem <size>\t\tuse <size> physical memory for caches.  Use K, M or G to\n\t\t\tspecify Kbytes, Mbytes or Gbytes respectively\n",
	"-mem-percent <percent>\tuse <percent> physical memory for caches.  Default 25%%\n",
	"-mem-default\t\tprint default memory usage in Mbytes\n",
	"\nFilesystem append options:\n",
	"-noappend\t\tdo not append to existing filesystem\n",
	"-root-becomes <name>\twhen appending source files/directories, make the\n\t\t\toriginal root become a subdirectory in the new root\n\t\t\tcalled <name>, rather than adding the new source items\n\t\t\tto the original root\n",
	"-no-recovery\t\tdo not generate a recovery file\n",
	"-recovery-path <name>\tuse <name> as the directory to store the recovery file\n",
	"-recover <name>\t\trecover filesystem data using recovery file <name>\n",
	"\nFilesystem actions options:\n",
	"-action <action@expr>\tevaluate <expr> on every file, and execute <action>\n\t\t\tif it returns TRUE\n",
	"-log-action <act@expr>\tas above, but log expression evaluation results and\n\t\t\tactions performed\n",
	"-true-action <act@expr>\tas above, but only log expressions which return TRUE\n",
	"-false-action <act@exp>\tas above, but only log expressions which return FALSE\n",
	"-action-file <file>\tas action, but read actions from <file>\n",
	"-log-action-file <file>\tas -log-action, but read actions from <file>\n",
	"-true-action-file <f>\tas -true-action, but read actions from <f>\n",
	"-false-action-file <f>\tas -false-action, but read actions from <f>\n",
	"\nTar file only options:\n",
	"-default-mode <mode>\ttar files often do not store permissions for\n\t\t\tintermediate directories.  This option sets the default\n\t\t\tdirectory permissions to octal <mode>, rather than 0755.\n\t\t\tThis also sets the root inode mode\n",
	"-default-uid <value>\ttar files often do not store uids for intermediate\n\t\t\tdirectories.  This option sets the default directory\n\t\t\towner to <value>, rather than the user running\n\t\t\tMksquashfs.  <value> can be either an integer uid or\n\t\t\tuser name.  This also sets the root inode uid\n",
	"-default-gid <value>\ttar files often do not store gids for intermediate\n\t\t\tdirectories.  This option sets the default directory\n\t\t\tgroup to <value>, rather than the group of the user\n\t\t\trunning Mksquashfs.  <value> can be either an integer\n\t\t\tuid or group name.  This also sets the root inode gid\n",
	"-ignore-zeros\t\tallow tar files to be concatenated together and fed to\n\t\t\tMksquashfs.  Normally a tarfile has two consecutive 512\n\t\t\tbyte blocks filled with zeros which means EOF and\n\t\t\tMksquashfs will stop reading after the first tar file on\n\t\t\tencountering them. This option makes Mksquashfs ignore\n\t\t\tthe zero filled blocks\n",
	"\nExpert options (these may make the filesystem unmountable):\n",
	"-nopad\t\t\tdo not pad filesystem to a multiple of 4K\n",
	"-offset <offset>\tskip <offset> bytes at the beginning of FILESYSTEM.\n\t\t\tOptionally a suffix of K, M or G can be given to specify\n\t\t\tKbytes, Mbytes or Gbytes respectively.\n\t\t\tDefault 0 bytes\n",
	"-o <offset>\t\tsynonym for -offset\n",
	"\nMiscellaneous options:\n",
	"-fstime <time>\t\talternative name for -mkfs-time\n",
	"-always-use-fragments\talternative name for -tailends\n",
	"-root-owned\t\talternative name for -all-root\n",
	"-noInodeCompression\talternative name for -noI\n",
	"-noIdTableCompression\talternative name for -noId\n",
	"-noDataCompression\talternative name for -noD\n",
	"-noFragmentCompression\talternative name for -noF\n",
	"-noXattrCompression\talternative name for -noX\n",
	"\n",
	"-help\t\t\toutput this options text to stdout\n",
	"-h\t\t\toutput this options text to stdout\n",
	"\n",
	"-Xhelp\t\t\tprint compressor options for selected compressor\n",
	"\nPseudo file definition format:\n",
	"\"filename d mode uid gid\"\t\tcreate a directory\n",
	"\"filename m mode uid gid\"\t\tmodify filename\n",
	"\"filename b mode uid gid major minor\"\tcreate a block device\n",
	"\"filename c mode uid gid major minor\"\tcreate a character device\n",
	"\"filename f mode uid gid command\"\tcreate file from stdout of command\n",
	"\"filename s mode uid gid symlink\"\tcreate a symbolic link\n",
	"\"filename i mode uid gid [s|f]\"\t\tcreate a socket (s) or FIFO (f)\n",
	"\"filename x name=val\"\t\t\tcreate an extended attribute\n",
	"\"filename l linkname\"\t\t\tcreate a hard-link to linkname\n",
	"\"filename L pseudo_filename\"\t\tsame, but link to pseudo file\n",
	"\"filename D time mode uid gid\"\t\tcreate a directory with timestamp time\n",
	"\"filename M time mode uid gid\"\t\tmodify a file with timestamp time\n",
	"\"filename B time mode uid gid major minor\"\n\t\t\t\t\tcreate block device with timestamp time\n",
	"\"filename C time mode uid gid major minor\"\n\t\t\t\t\tcreate char device with timestamp time\n",
	"\"filename F time mode uid gid command\"\tcreate file with timestamp time\n",
	"\"filename S time mode uid gid symlink\"\tcreate symlink with timestamp time\n",
	"\"filename I time mode uid gid [s|f]\"\tcreate socket/fifo with timestamp time\n",
	NULL};


void print_options(FILE *stream, char *name)
{
	int i;

	fprintf(stream, "SYNTAX:%s source1 source2 ...  FILESYSTEM [OPTIONS] ", name);
	fprintf(stream, "[-e list of\nexclude dirs/files]\n");

	for(i = 0; options_text[i] != NULL; i++)
		fprintf(stream, options_text[i]);

	fprintf(stream, "\nCompressors available and compressor specific options:\n");

	display_compressor_usage(stream, COMP_DEFAULT);

	fprintf(stream, "\nEnvironment:\n");
	fprintf(stream, "SOURCE_DATE_EPOCH\tIf set, this is used as the ");
	fprintf(stream,	"filesystem creation\n");
	fprintf(stream, "\t\t\ttimestamp.  Also any file timestamps which are\n");
	fprintf(stream, "\t\t\tafter SOURCE_DATE_EPOCH will be clamped to\n");
	fprintf(stream, "\t\t\tSOURCE_DATE_EPOCH.  See\n");
	fprintf(stream, "\t\t\thttps://reproducible-builds.org/docs/source-date-epoch/\n");
	fprintf(stream, "\t\t\tfor more information\n");
	fprintf(stream, "\nExit status:\n");
	fprintf(stream, "  0\tMksquashfs successfully generated a filesystem.\n");
	fprintf(stream, "  1\tFatal errors occurred, Mksquashfs aborted and ");
	fprintf(stream, "did not generate a\n\tfilesystem (or update if ");
	fprintf(stream, "appending).\n");
	fprintf(stream, "\nSee also:");
	fprintf(stream, "\nThe README for the Squashfs-tools 4.6.1 release, ");
	fprintf(stream, "describing the new features can be\n");
	fprintf(stream, "read here https://github.com/plougher/squashfs-tools/blob/master/README-4.6.1\n");
	fprintf(stream, "\nThe Squashfs-tools USAGE guide can be read here\n");
	fprintf(stream, "https://github.com/plougher/squashfs-tools/blob/master/USAGE-4.6\n");
	fprintf(stream, "\nThe ACTIONS-README file describing how to use the new actions feature can be\n");
	fprintf(stream, "read here https://github.com/plougher/squashfs-tools/blob/master/ACTIONS-README\n");
}


int print_option(FILE *stream, char *option)
{
	int i;

	for(i = 0; options[i] != NULL || options[i + 1] != NULL; i++) {
		if(options[i] && strcmp(option, options[i]) == 0) {
			fprintf(stream, options_text[i]);
			return TRUE;
		}
	}

	return FALSE;
}
