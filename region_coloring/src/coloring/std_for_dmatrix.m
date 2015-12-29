function r = std_for_dmatrix(D) 
N = size(D,1); 
D = reshape(D, N*N,1); 
m = sum(D)/(N*(N-1));
r = ((sum((D-m).^2))/(N*(N-1)-1))^0.5;  

