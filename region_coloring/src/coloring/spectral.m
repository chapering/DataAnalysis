function Y = spectral(D, dim)
%
% Simple spectral embedding 
%
if(nargin < 2)
  dim = 2; 
elseif(nargin < 1)
  error('you have to provide a similarity matrix D to be embedded'); 
end

N = size(D,1);
L = D.*D;
clear D; 
fprintf(1, ' formed L\n');
gamma = eye(N,N) - (1/N).*ones(N,1)*(ones(N,1)');
fprintf(1, ' formed gamma\n');
M = (-1/2) * gamma * L * gamma;
clear L; % done with L so we Free it.
clear gamma; % likewise w/ gamma 
fprintf(1, ' calling eig\n');
%options.issym = 1; 
options.isreal= 1;

%[U lambda] = eigs(M,dim,'LM', options);
[U lambda] = eig(M); 

% are these ordered correctly?
lambda = diag(lambda);
Y = zeros(N, dim);
for i = 1:dim
    Y(:,i) = sqrt(lambda(i)) * U(:,i);
end


