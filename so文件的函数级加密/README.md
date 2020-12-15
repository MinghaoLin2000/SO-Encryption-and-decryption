# 加密流程总结
## 前言:  
so文件中的符号结构体都在.dynsym，符号名都在.dynstr，同时符号结构体在.dynsym中包含了st_name(.dynstr的索引),st_value(符号在文件中的偏移),st_size(函数的大小)，也就是我们找到了.dynsym section，再找到对应的符号结构体，不就可以进行一波函数级加密了

## 加密流程
1. 找到.dynsym section  
这里就遇到一个问题，就是之前在段加密时，我们是通过.shstrtab来找到我们想找的节，然而在.shstrtab并没有dynsym这个节的名字，所以需要找到别的法子，书上写一种通过type来找，可是同一种类型的section有多个，这种方法不适合，所以切换了另外一种视角从装载的视角去考虑问题，有一个叫.dynamic的segment，所有.dymstr,.dymsym,.hash section都在这个段里，那我们只需要找到对应segment里面的p_shoff等，就可以找到对应的segment了，但是如何区别这几个段呢？主要是通过这个segmeng中的结构体来进行操作，如下面
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
d_tag:标示，这个dyn类型，是.dynsym还是.dynstr等
d_val:section的大小
d_ptr:section的偏移地址
```
那么问题不就解决了  
2. 找到符号结构体
这里又引入.hash这个section了，这个是真的神奇，这个section是由bucket数组以及chain数组构成了，而这些数组中又存着.dynsym的索引，对应一个公式，函数名经过hash后的值x，bucket[x%nbucket]返回值的y为.dynsym的函数符号索引，不过hash是有可能一样的，所以为了保险，所以引入chain这个数组，其实就是一个链表，每个值都可能是真正的函数符号结构体，实际得根据字符串是否相等作比较  
```
for(i=bucket[funHash%nbucket];i!=0;i=chain[i])
{
    if(strcmp(dynstr+(funSym+i)->st_name,funcName)==0)
    {
        flag=0;
        break;
    }
}
```

3. 然后就是和so文件特定段加密过程一样的了，没啥好说的了