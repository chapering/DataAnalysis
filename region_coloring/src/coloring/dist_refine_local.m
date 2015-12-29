function X = dist_refine_local(D,X,T,c,numiter)
%
% INPUT: 
%
% D - original distance NxN  matrix 
% X - reduced dxN coordinates (columns of observations) 

% OUTPUT: 
%
% compute distance matrix in reduced coordinates 

%err_vis(D, X, t, T); 
%d= L2_distance(X, X, 1); 
N= size(D,1); 

% compute new adjecency matrix 
a = zeros(N,N); 
a = (D<T);  

% line-end coordinates for visualization 
% figure;
% hold on; 
%max_ = -Inf; 
%err_ = Inf;  
for iter=1:numiter
  fprintf(1,'iteration# %d\n', iter); 
  for i=1:N
    for j=i+1:N 
      if(a(i,j)>0)
        d = norm(X(:,i) - X(:,j));
        e = D(i,j) - d;
         if(abs(e) > eps)
           t = e * c; 
           if(d == 0 )
             theta = 2*pi*rand(1);
             X(:,i) = X(:,i) + t*[cos(theta) sin(theta)];
             theta = 2*pi*rand(1);
             X(:,j) = X(:,j) + t*[cos(theta) sin(theta)];
           else
             X(:,i) = X(:,i)+(0.5*t*normc(X(:,i) - X(:,j)));
             X(:,j) = X(:,j)+(0.5*t*normc(X(:,j) - X(:,i)));
         end
       end
     end 
   end 
 end
end 
disp('done!');

