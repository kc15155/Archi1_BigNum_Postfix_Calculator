#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
 
# define MAX_LEN 100     /* Maximal line size */

 extern int mul (char * a, long aa, char * b, long bb, char * c);
 extern int add (char * a, long aa, char * b, long bb, char * c);
 extern int subt (char *  a, long aa, char * b, long bb, char * c);
 
 typedef struct bignum {
    int negative;
    long number_of_digits;
    char *digit;
    } bignum;

struct bignum createBig (long num, char* dig, int negative)
{
    struct bignum toadd;
    toadd.number_of_digits=num;
    toadd.digit=dig;
    toadd.negative=negative;
    return toadd;
}

struct Stack
{
    int top;
    unsigned capacity;
    bignum * array;
};


struct Stack* createStack(unsigned capacity)
{
    struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array =  malloc(stack->capacity * sizeof(bignum));
    return stack;
}
 

int isFull(struct Stack* stack)
{   return stack->top == stack->capacity - 1; }
 

int isEmpty(struct Stack* stack)
{   return stack->top == -1;  }
 

void push(struct Stack* stack, bignum item)
{
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
}
 

bignum pop(struct Stack* stack)
{
    if (!isEmpty(stack))
    return stack->array[stack->top--];
    bignum undefined;
    return undefined;
}

int whosBigger (bignum a, bignum b)
{
    if (a.number_of_digits>b.number_of_digits)
        return 1;
    if (a.number_of_digits<b.number_of_digits)
        return -1;
    if (a.number_of_digits==b.number_of_digits)
    {
        for (int i=0; i<a.number_of_digits; i++)
        {
            if (a.digit[i]>b.digit[i])
                return 1;
            if (a.digit[i]<b.digit[i])
                return -1;
        }
    }
    return 0;
}

void shrink (bignum * toShrink)
{
    if (toShrink->digit[0]=='0')
             {
                 while (toShrink->digit[0]=='0' && toShrink->number_of_digits>1)
                 {
                    toShrink->number_of_digits--;
                    char * newdig = (char *) malloc (toShrink->number_of_digits+1);
                    newdig[toShrink->number_of_digits]='\0';
                    for (int i=0; i<toShrink->number_of_digits; i++)
                        newdig[i]=toShrink->digit[i+1];
                    toShrink->digit=newdig;
                }
             }
}

int main(int argc, char **argv) {
    
  bignum result; 
  int c;
  int firstd=1;
  struct Stack* stack = createStack(1024);
  struct bignum toadd;
  char * newnum;
  int flag=0;
  do {
      c = getchar();
      if (c=='_')
          flag=1;
      if(c=='q') 
         break ;
      if (c=='p')
      {
           if (firstd==0)
             {
                 push(stack,toadd);
                 firstd=1;
                 flag=0;
             }
          bignum p = stack -> array[stack->top];
          if (p.negative==1)
              putchar('-');
          printf("%s\n",(p.digit));
        }
      if (c=='c')
      {
        while (!isEmpty(stack))
            pop(stack);
       }
      if (c>=48 && c<=57)
      {
          if (firstd==1)
          {
            firstd=0;  
            newnum = (char *) malloc (1);
            newnum[0]=c;
            toadd = createBig(1,newnum,flag);
          }
          else
          {
            newnum = (char *) realloc (newnum,toadd.number_of_digits+1);  
            toadd.digit[toadd.number_of_digits]=c;
            toadd.number_of_digits++;
            }
        }
      if (c<=32)
      {
          if (firstd==0)
          push(stack,toadd);
          firstd=1;  
          flag=0;
        }
      else
      {
         if (c=='+')
            {
             if (firstd==0)
             {
                 push(stack,toadd);
                 firstd=1;
                 flag=0;
             }
             bignum a = pop(stack);
             bignum b = pop(stack);
             bignum c;
             long maxdigits;
             int bigger = whosBigger(a,b);
             if (bigger==1)
             {  maxdigits=a.number_of_digits+2;
                char * newb = (char *) malloc (a.number_of_digits+2);
                newb[0]='0';
                newb[a.number_of_digits+1]='\0';
                for (int i=1 ; i<=((a.number_of_digits)-(b.number_of_digits)) ; i++)
                    newb[i]='0';
                for (int j=0; j<b.number_of_digits; j++)
                    newb[(a.number_of_digits-b.number_of_digits)+1+j]=b.digit[j];
                b.digit=newb;
                char * newa = (char *) malloc (a.number_of_digits+2);
                newa[0]='0';
                newa[a.number_of_digits+1]='\0';
                for (int i=0 ; i<a.number_of_digits; i++)
                    newa[i+1]=a.digit[i];
                a.digit=newa;
                if (a.negative==0 && b.negative==0)
                            result.negative=0;
                if (a.negative==0 && b.negative==1)
                            result.negative=0;
                if (a.negative==1 && b.negative==0)
                            result.negative=1;
                if (a.negative==1 && b.negative==1)
                            result.negative=1;
                }
                if (bigger<=0)
                {
                 maxdigits=b.number_of_digits+2;
                 char * newa = (char *) malloc (b.number_of_digits+2);
                 newa[0]='0';
                 newa[b.number_of_digits+1]='\0';
                 for (int i=1 ; i<=((b.number_of_digits)-(a.number_of_digits)) ; i++)
                    newa[i]='0';
                 for (int j=0; j<a.number_of_digits; j++)
                    newa[(b.number_of_digits-a.number_of_digits)+1+j]=a.digit[j];
                 a.digit=newa;
                 char * newb = (char *) malloc (b.number_of_digits+2);
                 newb[0]='0';
                 newb[b.number_of_digits+1]='\0';
                 for (int i=0 ; i<b.number_of_digits; i++)
                    newb[i+1]=b.digit[i];
                 b.digit=newb;
                }
                 if (bigger==-1)
                    {
                     c=b;
                     b=a;
                     a=c;
                     if (a.negative==0 && b.negative==0)
                            result.negative=0;
                     if (a.negative==0 && b.negative==1)
                            result.negative=0;
                    if (a.negative==1 && b.negative==0)
                            result.negative=1;
                    if (a.negative==1 && b.negative==1)
                            result.negative=1;
                    }
                        if (bigger==0)
                        {
                            if (a.negative==1 && b.negative==1)
                                result.negative=1;
                            else
                                result.negative=0;
                        }
             result.digit=(char *) malloc ((int)maxdigits);
             for (int i=0; i<(int)maxdigits-1 ; i++)
             {
                result.digit[i]='0'; 
                }
             result.digit[(int)maxdigits-1]='\0';
             if (a.negative==0 && b.negative==0)
                 result.number_of_digits = add (a.digit,(int)maxdigits-2,b.digit,(int)maxdigits-2,result.digit+((int)maxdigits-2));
             if (a.negative==0 && b.negative==1)
                 result.number_of_digits = subt (a.digit,(int)maxdigits-2,b.digit,(int)maxdigits-2,result.digit+((int)maxdigits-2));
             if (a.negative==1 && b.negative==0)
                 result.number_of_digits = subt (a.digit,(int)maxdigits-2,b.digit,(int)maxdigits-2,result.digit+((int)maxdigits-2));
             if (a.negative==1 && b.negative==1)
                 result.number_of_digits = add (a.digit,(int)maxdigits-2,b.digit,(int)maxdigits-2,result.digit+((int)maxdigits-2));
             shrink(&result);
             toadd = result;
             push(stack,toadd);
             }
            if (c=='-')
             {
              if (firstd==0)
              {
                 push(stack,toadd);
                 firstd=1;
                 flag=0;
              }
              bignum a = pop(stack);
              bignum b = pop(stack);
              bignum c;
              int bigger = whosBigger(a,b);
                if (bigger==1)  
                {
                        if (a.negative==0 && b.negative==0)
                            result.negative=1;
                        if (a.negative==0 && b.negative==1)
                            result.negative=1;
                        if (a.negative==1 && b.negative==0)
                            result.negative=0;
                        if (a.negative==1 && b.negative==1)
                            result.negative=0;
                }
                if (bigger==-1) 
                {
                    if (a.negative==0 && b.negative==0)
                            result.negative=0;
                    if (a.negative==0 && b.negative==1)
                            result.negative=1;
                    if (a.negative==1 && b.negative==0)
                            result.negative=0;
                    if (a.negative==1 && b.negative==1)
                            result.negative=1;
                    c=b;
                    b=a;
                    a=c;
                }
                        if (bigger==0)
                        {
                            if (b.negative==1 && a.negative==0)
                                result.negative=1;
                            else
                                result.negative=0;
                        }
              long maxdigits;
              if (a.number_of_digits > b.number_of_digits)
             {  maxdigits=a.number_of_digits+2;
                char * newb = (char *) malloc (a.number_of_digits+2);
                newb[0]='0';
                newb[a.number_of_digits+1]='\0';
                for (int i=1 ; i<=((a.number_of_digits)-(b.number_of_digits)) ; i++)
                    newb[i]='0';
                for (int j=0; j<b.number_of_digits; j++)
                    newb[(a.number_of_digits-b.number_of_digits)+1+j]=b.digit[j];
                b.digit=newb;
                char * newa = (char *) malloc (a.number_of_digits+2);
                newa[0]='0';
                newa[a.number_of_digits+1]='\0';
                for (int i=0 ; i<a.number_of_digits; i++)
                    newa[i+1]=a.digit[i];
                a.digit=newa;
               }
             else
             { 
                 maxdigits=b.number_of_digits+2;
                 char * newa = (char *) malloc (b.number_of_digits+2);
                 newa[0]='0';
                 newa[b.number_of_digits+1]='\0';
                 for (int i=1 ; i<=((b.number_of_digits)-(a.number_of_digits)) ; i++)
                    newa[i]='0';
                 for (int j=0; j<a.number_of_digits; j++)
                    newa[(b.number_of_digits-a.number_of_digits)+1+j]=a.digit[j];
                 a.digit=newa;
                 char * newb = (char *) malloc (b.number_of_digits+2);
                 newb[0]='0';
                 newb[b.number_of_digits+1]='\0';
                 for (int i=0 ; i<b.number_of_digits; i++)
                    newb[i+1]=b.digit[i];
                 b.digit=newb;
                }
             result.digit=(char *) malloc ((int)maxdigits);
             for (int i=0; i<(int)maxdigits-1 ; i++)
                result.digit[i]='0'; 
             result.digit[(int)maxdigits-1]='\0';
             if (a.negative==0 && b.negative==0)
                 result.number_of_digits = subt (a.digit,(int)maxdigits-2,b.digit,(int)maxdigits-2,result.digit+((int)maxdigits-2));
             if (a.negative==0 && b.negative==1)
                 result.number_of_digits = add (a.digit,(int)maxdigits-2,b.digit,(int)maxdigits-2,result.digit+((int)maxdigits-2));
             if (a.negative==1 && b.negative==0)
                 result.number_of_digits = add (a.digit,(int)maxdigits-2,b.digit,(int)maxdigits-2,result.digit+((int)maxdigits-2));
             if (a.negative==1 && b.negative==1)
                 result.number_of_digits = subt (a.digit,(int)maxdigits-2,b.digit,(int)maxdigits-2,result.digit+((int)maxdigits-2));
             shrink(&result);
             toadd=result;
             push(stack,toadd);
             }
            if (c=='*')
            {
            if (firstd==0)
             {
                 push(stack,toadd);
                 firstd=1;
                 flag=0;
             }
            long maxdigits;
            bignum a=pop(stack);
            bignum b=pop(stack);
            bignum c;
            if (a.number_of_digits > b.number_of_digits)
            {
                maxdigits=a.number_of_digits+1;
                char * newb = (char *) malloc (maxdigits);
                newb[maxdigits-1]='\0';
                for (int i=0 ; i<((a.number_of_digits)-(b.number_of_digits)) ; i++)
                    newb[i]='0';
                for (int j=0; j<b.number_of_digits; j++)
                    newb[(a.number_of_digits-b.number_of_digits)+j]=b.digit[j];
                b.digit=newb;
                c=b;
                b=a;
                a=c;
                }
            else if (a.number_of_digits < b.number_of_digits)
                {
                maxdigits=b.number_of_digits+1;
                char * newa = (char *) malloc (maxdigits);
                newa[maxdigits-1]='\0';
                for (int i=0 ; i<((b.number_of_digits)-(a.number_of_digits)) ; i++)
                    newa[i]='0';
                for (int j=0; j<a.number_of_digits; j++)
                    newa[(b.number_of_digits-a.number_of_digits)+j]=a.digit[j];
                a.digit=newa;
                }
            if (a.number_of_digits == b.number_of_digits)
                maxdigits=a.number_of_digits+1;
            bignum result;
            result.digit = (char *) malloc (a.number_of_digits +b.number_of_digits +1);
            result.digit[a.number_of_digits+b.number_of_digits]='\0';
            for (int i=0; i<(a.number_of_digits+b.number_of_digits) ; i++)
                result.digit[i]='0';
             result.number_of_digits = mul (a.digit, maxdigits-2 ,b.digit, maxdigits-2, result.digit + (a.number_of_digits + b.number_of_digits -1));
             result.number_of_digits=a.number_of_digits+b.number_of_digits;
            shrink(&result);
            toadd=result;
            if (a.negative==b.negative)
              toadd.negative=0;
           else
               toadd.negative=1;
            push(stack,toadd);
           }
            
         if (c=='/')
            {
             if (firstd==0)
             {
                 push(stack,toadd);
                 firstd=1;
                 flag=0;
             }
             bignum b = pop(stack);
             bignum a = pop(stack);
             int bzero=0;
             for (int i=0; i<b.number_of_digits; i++)
             {
                if (b.digit[i]!='0')
                    bzero=1;
            }
            if (bzero==0)
                {
                printf("%s\n","Division by zero is NOT allowed!");
                break;
                }
             long maxdigits;
             int lessthan=0;
             int equal=1;
             int bigger = whosBigger(a,b);
             if (bigger==1)
             { 
                equal=0;
                maxdigits=a.number_of_digits+2;
                char * newb = (char *) malloc (a.number_of_digits+b.number_of_digits+1);
                newb[a.number_of_digits+b.number_of_digits]='\0';
                for (int i=0 ; i<=(a.number_of_digits) ; i++)
                    newb[i]='0';
                for (int j=0; j<b.number_of_digits; j++)
                    newb[a.number_of_digits+j]=b.digit[j];
                b.digit=newb;
                char * newa = (char *) malloc (a.number_of_digits+b.number_of_digits+1);
                newa[a.number_of_digits+b.number_of_digits]='\0';
                for (int i=0 ; i<b.number_of_digits; i++)
                    newa[i]='0';
                for (int i=0 ; i<a.number_of_digits; i++)
                    newa[b.number_of_digits+i]=a.digit[i];
                a.digit=newa;
                result.digit=(char *) malloc (a.number_of_digits+b.number_of_digits+1);
                for (int i=0; i<(a.number_of_digits+b.number_of_digits) ; i++)
                    result.digit[i]='0';
                result.digit[a.number_of_digits+b.number_of_digits]='\0';
             } 
                     if (bigger==-1)
                     {
                        equal=0;
                        lessthan=1;
                        }
             if (a.number_of_digits<b.number_of_digits)
                 lessthan=1;
             if (equal==1)
                 maxdigits=1;
             char * temp = (char *) malloc (a.number_of_digits+b.number_of_digits+1);
             for (int i=0; i<(a.number_of_digits+b.number_of_digits) ; i++)
                temp[i]='0';
             temp[a.number_of_digits+b.number_of_digits]='\0';
             char * onedummy = (char *) malloc (a.number_of_digits+b.number_of_digits+1);
             onedummy[a.number_of_digits+b.number_of_digits-1]='1';
             onedummy[a.number_of_digits+b.number_of_digits]='\0';
             int multiflag=0;
             for (int i=0; i<a.number_of_digits+b.number_of_digits-1; i++)
                 onedummy[i]='0';
             bignum output;
             char * outputstring = (char *) malloc (a.number_of_digits+b.number_of_digits+1);
             outputstring[a.number_of_digits+b.number_of_digits]='\0';
             char * tempoutput = (char *) malloc (a.number_of_digits+b.number_of_digits+1);
             for (int i=0; i<a.number_of_digits+b.number_of_digits; i++)
                 tempoutput[i]='0';
             tempoutput[a.number_of_digits+b.number_of_digits]='\0';
             for (int i=0; i<a.number_of_digits+b.number_of_digits; i++)
                 outputstring[i]='0';
             char * acc = (char *) malloc (a.number_of_digits+b.number_of_digits+1);
             acc[a.number_of_digits+b.number_of_digits]='\0';
             for (int i=0; i<a.number_of_digits+b.number_of_digits; i++)
                 acc[i]='0';
             bignum accu = createBig(maxdigits,acc,0);
             output=createBig(a.number_of_digits+b.number_of_digits,outputstring,0);
             char * multi = (char *) malloc (a.number_of_digits+b.number_of_digits+1);
             multi[a.number_of_digits+b.number_of_digits]='\0';
             for (int i=0 ; i<a.number_of_digits+b.number_of_digits ; i++)
                 multi[i]='0';
             multi[b.number_of_digits]='1';
             int bigflag=1;
            if (lessthan==0 && equal==0) 
            { 
             while (bigflag==1)
             {
                 multiflag=0;
                 for (int i=0; i<a.number_of_digits+b.number_of_digits ; i++)
                    temp[i]=accu.digit[i];
                 for (int i=0; i<a.number_of_digits+b.number_of_digits ; i++)
                    result.digit[i]='0';
                 result.number_of_digits = mul (b.digit, (a.number_of_digits+b.number_of_digits-1), multi, (a.number_of_digits+b.number_of_digits-1), result.digit + (a.number_of_digits + b.number_of_digits -1));
                 accu.number_of_digits = add(accu.digit, a.number_of_digits+b.number_of_digits-1, result.digit,a.number_of_digits+b.number_of_digits-1, accu.digit + (a.number_of_digits+b.number_of_digits-1));
                 accu.number_of_digits=a.number_of_digits+b.number_of_digits;
                 if (accu.digit[0]=='0')
                 {
                     int i=0;
                 while (accu.digit[i]=='0' && accu.number_of_digits>1)
                  {
                    accu.number_of_digits--;
                    i++;
                  }
                }
                if (accu.number_of_digits>a.number_of_digits)
                {
                  for (int i=0; i<a.number_of_digits+b.number_of_digits ; i++)
                    accu.digit[i]=temp[i];  
                  for (int i=a.number_of_digits+b.number_of_digits-2; i>=0; i--)
                      multi[i+1]=multi[i];
                  multi[0]='0';  
                }
                if (accu.number_of_digits<a.number_of_digits)
                {
                    tempoutput[0]='0';
                    for (int i=0; i<a.number_of_digits+b.number_of_digits ; i++)
                        tempoutput[i]=output.digit[i];
                    output.number_of_digits = add(output.digit, a.number_of_digits+b.number_of_digits-1, multi,a.number_of_digits+b.number_of_digits-1, tempoutput + (a.number_of_digits+b.number_of_digits-1));
                    for (int i=0; i<a.number_of_digits+b.number_of_digits ; i++)
                        output.digit[i]=tempoutput[i];
                }
                if (accu.number_of_digits == a.number_of_digits)
                {
                    int flag=1;
                    for (int i=0; i<a.number_of_digits+b.number_of_digits && flag==1; i++)
                            {
                                if (a.digit[i]>accu.digit[i])
                                { 
                                    tempoutput[0]='0';
                                    for (int i=0; i<a.number_of_digits+b.number_of_digits ; i++)
                                        tempoutput[i]=output.digit[i];
                                    output.number_of_digits = add(output.digit, a.number_of_digits+b.number_of_digits-1, multi,a.number_of_digits+b.number_of_digits-1, tempoutput + (a.number_of_digits+b.number_of_digits-1));
                                    for (int i=0; i<a.number_of_digits+b.number_of_digits ; i++)
                                        output.digit[i]=tempoutput[i];
                                    flag=0;
                                }
                                if (a.digit[i]<accu.digit[i])
                                {
                                   for (int i=0; i<a.number_of_digits+b.number_of_digits ; i++)
                                        accu.digit[i]=temp[i];  
                                   for (int i=a.number_of_digits+b.number_of_digits-2; i>=0; i--)
                                     multi[i+1]=multi[i];
                                   multi[0]='0';  
                                   flag=0;
                                }
                            }
                            
                    if (flag==1) 
                    {
                      tempoutput[0]='0';
                     for (int i=0; i<a.number_of_digits+b.number_of_digits ; i++)
                        tempoutput[i]=output.digit[i];
                     output.number_of_digits = add(output.digit, a.number_of_digits+b.number_of_digits-1, multi,a.number_of_digits+b.number_of_digits-1, tempoutput + (a.number_of_digits+b.number_of_digits-1));
                     for (int i=0; i<a.number_of_digits+b.number_of_digits ; i++)
                        output.digit[i]=tempoutput[i];  
                      bigflag=0;
                    }
                    for(int i=0; i<a.number_of_digits+b.number_of_digits ; i++){
                        if(multi[i] != '0')
                            multiflag=1;
                    }
                    if(multiflag==0)
                        bigflag=0;
                }
               }
            }
            else 
            {
                if (equal==1 && lessthan==0)
                    output.digit=onedummy;
            }
            shrink(&output);
            toadd=output; 
           if (a.negative!=b.negative && lessthan==0)
              toadd.negative=1;
           else
               toadd.negative=0;
           push(stack,toadd);  
        }
   }
  }while(1);
  return 0;
}