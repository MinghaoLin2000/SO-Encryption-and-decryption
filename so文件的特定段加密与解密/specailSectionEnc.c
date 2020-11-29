#include<elf.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>  //create or open file if you use 
#include<stdlib.h>
void main(int argc,char* argv[])
{
    int fd;
    char *shstr=NULL;
    char *content=NULL; //encrypto section content
    Elf32_Ehdr ehdr; //the file header struct
    Elf32_Shdr shdr; //the section struct;
    char* specialSection=".anti"; // crypto section name;
    unsigned int base,length;
    unsigned short nblock,nsize;
    unsigned char block_size=16;
    int i=0;

  if(argc<2)
  {
      printf("You hadn't input the file");
      return;
  }
  fd=open(argv[1],O_RDWR);
  if(fd<0)
  {
      printf("open the input file failed!,try again!");
      return;
  }
  if(read(fd,&ehdr,sizeof(Elf32_Ehdr))!=sizeof(Elf32_Ehdr)) //if return value don't equal sizeof(ehdr),exit just mean fail 
    {
        printf("read file fail!");
        return;
    }
    //change the file pointer
 lseek(fd,ehdr.e_shoff+sizeof(Elf32_Shdr)*ehdr.e_shstrndx,SEEK_SET);
  if(read(fd,&shdr,sizeof(Elf32_Shdr))!=sizeof(Elf32_Shdr))
  {
      printf("open the section string table fail");
      return;
  }
  if((shstr=(char*)malloc(shdr.sh_size))==NULL)
  {
      printf("Malloc space for section string table failled");
      return;
  }
  lseek(fd,shdr.sh_offset,SEEK_SET);
  if(read(fd,shstr,shdr.sh_size)!=shdr.sh_size)
  {
      printf("Read string table failed!");
      return;
  }
  lseek(fd,ehdr.e_shoff,SEEK_SET);
  for(i=0;i<ehdr.e_shnum;i++)
  {
      if(read(fd,&shdr,sizeof(Elf32_Shdr))!=sizeof(Elf32_Shdr))
      {
          printf("find section failed!");
          return;
      }
      if(strcmp(shstr+shdr.sh_name,specialSection)==0)
      {
          base=shdr.sh_offset;
          length=shdr.sh_size;
          printf("find section %s\n",specialSection);
          break;
      }
  }
  lseek(fd,base,SEEK_SET);
    content=(char*)malloc(length);
    if(content==NULL)
    {
        printf("malloc space for content failed");
        return;
    }
    if((read(fd,content,length))!=length)
    {
        printf("Read section faild!");
        return;
    }
    nblock=length/block_size;
    nsize=base/4096+(base%4096==0?1:0);
    ehdr.e_entry=(length<<16)+nsize;
    ehdr.e_shoff=base;
    for(i=0;i<length;i++)
    {
        content[i]=~content[i];
    }
    lseek(fd,0,SEEK_SET);
    if(write(fd,&ehdr,sizeof(Elf32_Ehdr))!=sizeof(Elf32_Ehdr))
    {
        printf("write ELFhead to. so failed");
        return;
    }
    lseek(fd,base,SEEK_SET);
    if(write(fd,content,length)!=length)
    {
        printf("write modified content to .so failed");
        return;
    }
    printf("completed");
    free(content);
    free(shstr);
    close(fd);
    return;

    


  
}