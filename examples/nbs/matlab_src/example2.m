load('input');
    
%Run the NBS with the following parameter options: 
%Set an appropriate threshold. It is difficult to provide a rule of thumb 
%to guide the choice of this threshold. Trial-and-error is always an option
%with the number of permutations generated per trial set low. 
THRESH=2; 
%Generate 10 permuations. Many more permutations are required in practice
%to yield a reliable estimate. 
K=100;
%Set TAIL to left, and thus test the alternative hypothesis that mean of 
%population X < mean of population Y
TAIL='left'; 
%Run the NBS
[pval,adj]=nbs(X,Y,THRESH,K,TAIL); 
