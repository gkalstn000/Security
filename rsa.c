/*
 * @file    rsa.c
 * @author  하민수 / 2014038422
 * @date    작성 일자
 * @brief   mini RSA implementation code
 * @details Mod(llint a, llint b) 함수 새로 정의
 *          GCD함수 while문대신 Mod함수 사용
 *          나누기 함수 새로 정의
 *          div_(llint x, llint y)
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rsa.h"
#define llint long long int
typedef unsigned char byte;
#define true 1;
#define false 0;
#include <limits.h>

llint p, q, e, d, n;

/*
 * @param    a : 피연산자
 * @param    n : 모듈러 n값
 * 피연산자가 n보다 크면 모듈러 연산 해주는 함수
 */

llint Mod(llint a, llint n) {
    llint s = n;
    //n이 a보다 작을동안 계속 2를 곱해준다
    while (s <= a) {
        s <<= 1;
    }
    llint r = a;
    
    //a에서 n을 계속 빼준다
    while (r >= n) {
        s >>= 1;
        if (s <= r) {    
            r -= s;
        }
    }
    return r;  
}    

/*
 * @brief     모듈러 덧셈 연산을 하는 함수.
 * @param     llint a     : 피연산자1.
 * @param     llint b     : 피연산자2.
 * @param     byte op    : +, - 연산자.
 * @param     llint n      : 모듈러 값.
 * @return    llint result : 피연산자의 덧셈에 대한 모듈러 연산 값. (a op b) mod n
 * @todo      모듈러 값과 오버플로우 상황을 고려하여 작성한다.
 */
llint ModAdd(llint a, llint b, byte op, llint n) {
    //+ : 43, - : 45
    //빼기면 b만 음수로바꿔주면됨
    if(op == 45) { b = -b;}

    llint result = Mod(a, n) - (n - Mod(b, n));
    //result가 음수면 n을 더해준다
    if(result < 0) { result += n; }

    //예외처리(op가 43(+) or 45(-) 일때,  a, b숫자 아닐때)

    return result;
}

/*
 * @brief      모듈러 곱셈 연산을 하는 함수.
 * @param      llint x       : 피연산자1.
 * @param      llint y       : 피연산자2.
 * @param      llint n       : 모듈러 값.
 * @return     llint result  : 피연산자의 곱셈에 대한 모듈러 연산 값. (a x b) mod n
 * @todo       모듈러 값과 오버플로우 상황을 고려하여 작성한다.
 */
llint ModMul(llint x, llint y, llint n) {
    llint result = 0;

    x = Mod(x, n); y = Mod(y, n);
    for(;;) {
       //x의 마지막 비트 자리수가 1이면(홀수일 경우) result에 y를 더해준다
        if(x & 0x01) result = ModAdd(result, y, '+', n);

        x = Mod(x>>1, n);
        y = Mod(y<<1, n);

        //x가 0이하가 되면 무한루프 탈출
        if(x <= 0)  break;
    }
    return Mod(result, n);
}



/*
 * @brief      나눗셈 연산을 하는 함수.
 * @param      llint x       : 피연산자1.
 * @param      llint y       : 피연산자2.
 * @return     llint result  : 피연산자의 나눗셈에 대한값
 * @todo       나누기 연산없이 나누기 계산
 */

llint div_(llint x, llint y) {
    llint result = 0;
    
    //x에 x%y를 빼서
    //x를 y의 배수로 만든다.
    x -=  Mod(x, y);

    //x - 2y를 해준다 0이하가 될때까지
    while(x > 0) {
        x -= y << 1;
        result += 2;
    }
    //x가 0이라면 x가 y의 짝수배라는뜻.  
    //x가 음수라면 x가 y의 홀수배이니 result 에서 1을 빼누다
    if(x < 0) result -= 1;
    
    return result;
}






/*
 * @brief      모듈러 거듭제곱 연산을 하는 함수.
 * @param      llint base   : 피연산자1.
 * @param      llint exp    : 피연산자2.
 * @param      llint n      : 모듈러 값.
 * @return     llint result : 피연산자의 연산에 대한 모듈러 연산 값. (base ^ exp) mod n
 * @todo       모듈러 값과 오버플로우 상황을 고려하여 작성한다.
               'square and multiply' 알고리즘을 사용하여 작성한다.
 */
llint ModPow(llint base, llint exp, llint n) {
    base = Mod(base, n);
    llint result = 1ULL;
    //exp가 0이면 1 return
    if(exp == 0) return result;

    while (exp > 0) {
        //exp가 홀수인 경우base를 result에 곱해준다
        if (exp & 0x01) result = ModMul(result, base, n);
        //base를 제곱한다
        base = ModMul(base, base, n);
        //exp / 2 해준다
        exp = exp >> 1;
    }
    return result;
}

/*
 * @brief      입력된 수가 소수인지 입력된 횟수만큼 반복하여 검증하는 함수.
 * @param      llint testNum   : 임의 생성된 홀수.
 * @param      llint repeat    : 판단함수의 반복횟수.
 * @return     llint result    : 판단 결과에 따른 TRUE, FALSE 값.
 * @todo       Miller-Rabin 소수 판별법과 같은 확률적인 방법을 사용하여,
               이론적으로 4N(99.99%) 이상 되는 값을 선택하도록 한다. 
 */

bool Miller(llint testNum, llint a) {
    // d = n - 1
    llint d = testNum - 1;

    //d가 짝수일동안d/2 반복 
    //a^(n-1) mod n = n - 1이면 true
    while (!(d & 0x01)) {
        if (ModPow(a, d, testNum) == testNum-1)
            return true;
        d = d >> 1;
    }
    //tmp = a^(n-1) mon n
    llint tmp = ModPow(a, d, testNum);
    // a^(n-1) mod n = 1 이거나 a^(n-1) mod n = n - 1 이면 합성수는 아니다
    return tmp == testNum-1 || tmp == 1;
}

bool IsPrime(llint testNum, llint repeat) {
    if(testNum <= 1) return false;
    //확인해볼 a의 리스트
    llint alist[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    
    for(llint i = 0; i < repeat; i++) {
        llint a = alist[i];
        if(!Miller(testNum, a)) return false;
    }
    return true;
    
}


/*
 * @brief       모듈러 역 값을 계산하는 함수.
 * @param       llint a      : 피연산자1.
 * @param       llint m      : 모듈러 값.
 * @return      llint result : 피연산자의 모듈러 역수 값.
 * @todo        확장 유클리드 알고리즘을 사용하여 작성하도록 한다.
 */


llint ModInv(llint a, llint m)
{

    //gcd(r0, r1) = gcd(r1, r2) = gcd(r2, r3) = ... = gcd(r_(n-2), 0)
    //s_i = s_(i-2) - s_(i-1)*q_i
    //t_i = t_(i-2) - t_(i-1)*q_i
    //r_(i-2) = r_i % r_(i-1)
    llint r, q, s, s1=1, s2=0, t, t1=0, t2=1, tmp = a;

    while(m)
    { 
        q = div_(a, m);
        r = Mod(a, m);
        s = s1 - q*s2;
        t = t1 - q*t2;

        a = m;
        m = r;
        s1 = s2;
        s2 = s;
        t1 = t2;
        t2 = t;
    }

    if(a == 1) //역원이 있음
    {
        if(t1 < 0) t1 += tmp;
        return t1;
    }

    return 0;
} 


/*
 * @brief     RSA 키를 생성하는 함수.
 * @param     llint *p   : 소수 p.
 * @param     llint *q   : 소수 q.
 * @param     llint *e   : 공개키 값.
 * @param     llint *d   : 개인키 값.
 * @param     llint *n   : 모듈러 n 값.
 * @return    void
 * @todo      과제 안내 문서의 제한사항을 참고하여 작성한다.
 */

void miniRSAKeygen(llint *p, llint *q, llint *e, llint *d, llint *n) {
    //llint가 가질수있는 최대값
    //곱셈을 못해서 ModMul을 해야하는데 값이 안없어지도록 최대값으로 mod k
    llint K = LONG_LONG_MAX;

    printf("mRSA key generator start.\n\n");

    //p, q값의 min-max 범위값
    //w는 랜덤값 들어갈 변수
    llint min = ModPow(2, 53>>1, K);
    llint max = ModPow(2, 64>>1, K);
    double w = 0;

    //p값 구하기
    while(1) {
        w = WELLRNG512a();
        *p = (w * (max-min)) + min;
        printf("randdom-number1 %lld selected.\n", *p);
        //소수판별
        if(IsPrime(*p, 12)) {
            printf("%lld may be Prime.\n\n", *p);
            break;
        }
        printf("%lld is not a Prime.\n\n", *p);
    }
     
    //q값 구하기
    while(1) { 
        w = WELLRNG512a();
        *q = (w * (max-min)) + min;
        printf("randdom-number2 %lld selected.\n", *q);
        //소수판별
        if(IsPrime(*q, 12)) {
            printf("%lld may be Prime.\n\n", *q);
            break;
        }
        printf("%lld is not a Prime.\n\n", *q);
    }
    // n = pq
    *n = ModMul(*p, *q, K);

    printf("finally selected prime p, q = %lld, %lld.\nthus, n = %lld\n\n", *p, *q, *n);

    //pi_n = (p-1)(q-1)
    llint pi_n = ModMul(*p-1, *q-1, *n);
    
    // 0 < e < pi_n
    *e = w * pi_n;


    //e값이 pi_n과 서로소 인지 확인
    while(1) {    
        w = WELLRNG512a();
        *e = w * pi_n;
        printf("e : %lld\n", *e);
        //서로소이면 break
        if(GCD(pi_n, *e)  == 1) break;

    }
    // ed = 1 mod pi_n인 e의 역수 구하기
    *d = ModInv(pi_n, *e);

    
    printf("d : %lld\n\n", *d);
    printf("e, d, n, pi_n : %lld, %lld, %lld, %lld\n\n", *e, *d, *n, pi_n);
    printf("e*d mod pi_n : %lld\n", ModMul(*e, *d, pi_n));
}


/*
 * @brief     RSA 암복호화를 진행하는 함수.
 * @param     llint data   : 키 값.
 * @param     llint key    : 키 값.
 * @param     llint n      : 모듈러 n 값.
 * @return    llint result : 암복호화에 결과값
 * @todo      과제 안내 문서의 제한사항을 참고하여 작성한다.
 */

//encryp = M^e mod n / decryp = encryp^d mod n
llint miniRSA(llint data, llint key, llint n) {
    llint data_pow = ModPow(data, key, n);
    return data_pow;    
}


llint GCD(llint a, llint b) {
    llint prev_a;

    while(b != 0) {
        printf("GCD(%lld, %lld)\n", a, b);
        prev_a = a;
        a = b;
        b = Mod(prev_a, b);
    }
    printf("GCD(%lld, %lld)\n\n", a, b);
    return a;
}

int main(int argc, char* argv[]) {
         
    byte plain_text[4] =  {0x12, 0x34, 0x56, 0x78};
    llint plain_data, encrpyted_data, decrpyted_data = 0;
    uint seed = time(NULL);

    memcpy(&plain_data, plain_text, 4);

    // 난수 생성기 시드값 설정
    seed = time(NULL);
    InitWELLRNG512a(&seed);
    // RSA 키 생성
    miniRSAKeygen(&p, &q, &e, &d, &n);
    printf("0. Key generation is Success!\n ");
    printf("p : %lld\n q : %lld\n e : %lld\n d : %lld\n N : %lld\n\n", p, q, e, d, n);
   
    
    
    // RSA 암호화 테스트
    encrpyted_data = miniRSA(plain_data, e, n);
    printf("1. plain text : %llu\n", plain_data);    
    printf("2. encrypted plain text : %llu\n\n", encrpyted_data);

    // RSA 복호화 테스트
    decrpyted_data = miniRSA(encrpyted_data, d, n);
    printf("3. cipher text : %lld\n", encrpyted_data);
    printf("4. Decrypted plain text : %lld\n\n", decrpyted_data);
    
    // 결과 출력
    printf("RSA Decryption: %s\n", (decrpyted_data == plain_data) ? "SUCCESS!" : "FAILURE!");
 
    return 0;
}
