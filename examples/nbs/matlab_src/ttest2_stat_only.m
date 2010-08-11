function t=ttest2_stat_only(x,y)

t=mean(x)-mean(y); 
n1=length(x); 
n2=length(y); 
s=sqrt(((n1-1)*var(x)+(n2-1)*var(y))/(n1+n2-2)); 
t=t/(s*sqrt(1/n1+1/n2)); 