unsigned short checkSum(unsigned short *dataPtr,int lenBytes)
{
    int quack;
    long sum;
    short answer;
    while(lenBytes > 1)
    {
        sum+=*dataPtr++;
        lenBytes-=2;
    }
    if (lenBytes == 1)
    {
        sum+=*((unsigned char*)dataPtr);
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum = sum + (sum >> 16);
    answer = (short)~sum;
    return answer;
}