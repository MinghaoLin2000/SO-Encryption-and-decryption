# 加密流程总结
## 前言:  
so文件中的符号结构体都在.dynsym，符号名都在.dynstr，同时符号结构体在.dynsym中包含了st_name(.dynstr的索引),st_value(符号在文件中的偏移),st_size(函数的大小)，也就是我们找到了.dynsym section，再找到对应的符号结构体，不就可以进行一波函数级加密了

## 加密流程
1. 找到.dynsym section  
这里就遇到一个问题，就是之前在段加密时，我们是通过.shstrtab来找到我们想找的节，然而在.shstrtab并没有dynsym这个节的名字，所以需要找到别的法子，书上写一种通过type来找，可是同一种类型的section有多个，这种方法不适合，
```
public static class elf32_dyn{
    public byte[] d_tag=new byte[4];
    public byte[] d_val=new byte[4];
    public byte[] d_ptr=new byte[4];
    public String toString()
    {
        return "d_tag:"+Utils.bytes2HexString(d_tag)+";d_un_d_val:"+
        Utils.bytes2HexString(d_val)+";d_un_d_ptr:"+Utils.byte2HexString(d_ptr);
    }
}
d_tag:标示，这个
```