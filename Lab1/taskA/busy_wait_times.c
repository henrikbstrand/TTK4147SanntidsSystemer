#include <sys/times.h>

int main(){

    struct tms buf;
    while(1){
        times(&buf);
        if(buf.tms_stime + buf.tms_utime >= 100){
            return 1;
        }
    }
    return 1;

}