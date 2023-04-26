#include "kernel/types.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/stat.h"

// Find first character after last slash.
char *fmtname(char *path)//从路径字符串中获取文件名
{
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;
    return p;
}
void equal_print(char *path, char *findname)
{

    if (strcmp(fmtname(path), findname) == 0)
        printf("%s\n", path);
}

void find(char *dir_name, char *file_name)
{
    int fd;
//打开，并将文件描述符给fd
    if ((fd = open(dir_name, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", dir_name);
        return;
    }
// struct stat {//文件的属性信息
//   int dev;     // 文件系统设备号
//   uint ino;    // Inode 值
//   short type;  // 文件类型
//   short nlink; // 文件被链接数
//   uint64 size; // 文件大小
// };
//struct stat st 在内存中分配了一块连续的空间，用于存储 struct stat 结构体的各个字段的值
//struct stat *st指针变量本身只占用一段很小的空间，但是它所指向的结构体仍然需要在内存中分配一段连续的空间来存储结构体的各个字段的值    

//fstat获取一个打开文件的状态信息;st存储查询结果
    struct stat *st = (struct stat *) malloc(sizeof(struct stat));
    if (fstat(fd, st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", dir_name);
        close(fd);
        return;
    }
// struct dirent {//目录属性
//     ino_t d_ino;           // 文件或目录的inode编号
//     off_t d_off;           // 文件或目录在目录中的偏移量
//     unsigned short d_reclen; // 该条记录的长度
//     unsigned char d_type;   // 文件或目录类型
//     char d_name[256];      // 文件或目录名称
// };

    //循环判断
    struct dirent de;
    //buf是用来记录文件前缀的，这样才会打印出之前的目录
    char buf[512], *p;
    switch (st->type)//根据打开文件描述符状态判断
    {
    case T_FILE://匹配查找的文件
        equal_print(dir_name, file_name);
        break;
    case T_DIR://打开的是目录
        if (strlen(dir_name) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: path too long\n");
            break;
        }
        //将path复制到buf里
        strcpy(buf, dir_name);
        //p为一个指针，指向buf(path)的末尾
        p = buf + strlen(buf);
        //在末尾添加/ 比如 path为 a/b/c 经过这步后变为 a/b/c/<-p
        *p++ = '/';
        // 如果是文件夹，则循环读这个文件夹里面的文件
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0 || (strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0))
                continue;
            //拼接出形如 a/b/c/de.name 的新路径(buf)
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            //递归查找
            find(buf, file_name);
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Usage: find <dirName> <fileName>\n");
        exit(-1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
