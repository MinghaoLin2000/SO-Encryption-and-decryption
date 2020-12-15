# 加密流程总结
## 前言:  
so文件中的符号结构体都在.dynsym，符号名都在.dynstr，同时符号结构体在.dynsym是以这种形式存在的
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