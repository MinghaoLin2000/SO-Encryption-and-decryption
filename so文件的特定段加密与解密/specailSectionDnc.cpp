#include <jni.h>
#include <string>
#include <unistd.h>
#include <elf.h>
#include <sys/mman.h>
jstring getString(JNIEnv* env) __attribute__((section (".anti")));
jstring getString(JNIEnv* env)
{
    return env->NewStringUTF("YenKoc is good and hard!");
}
void init_getString() __attribute__((constructor));
unsigned long getLibAddr()
{
    unsigned long ret=0;
    char name[]="libnative-lib.so";
    char buf[4096],*temp;
    int pid;
    FILE *fp;
    pid=getpid();
    sprintf(buf,"/proc/%d/maps",pid);
    fp=fopen(buf,"r");
    if(fp==NULL)
    {
        printf("open failed");
        return 0;
    }
    while (fgets(buf,sizeof(buf),fp))
    {
        if(strstr(buf,name))
        {
            temp=strtok(buf,"-");
            ret=strtoul(temp,NULL,16);
            break;
        }
    }
    fclose(fp);
    return ret;

}
void init_getString()
{
    char name[15];
    unsigned int nblock;
    unsigned int nsize;
    unsigned long base;
    unsigned long text_addr;
    unsigned int i;
    Elf32_Ehdr *ehdr;
    Elf32_Shdr *shdr;
    base =getLibAddr();
    ehdr = (Elf32_Ehdr *)base;
    text_addr=ehdr->e_shoff+base;
    nblock=ehdr->e_entry>>16;
    nsize=ehdr->e_entry&0xffff;

    if(mprotect((void *)base,4096*nsize,PROT_EXEC|PROT_READ|PROT_WRITE)!=0)
    {
        printf("mem privilege change failed");
    }
    for(i=0;i<nblock;i++)
    {
        char *addr=(char*)(text_addr+i);
        *addr=~(*addr);
    }
    if(mprotect((void *) base,4096*nsize,PROT_READ|PROT_EXEC)!=0)
    {
        printf("mem privilege change failed");
    }
    printf("Decrypto success");
}
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_sectionenctest1_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_sectionenctest1_MainActivity_getString(JNIEnv *env, jobject thiz) {
    return getString(env);
    // TODO: implement getString()
}
