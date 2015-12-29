function D=embed_distance_batch(prefix, num_of_models, dalg, dtype, rootdir)  
% 
% embed_distance_batch -  embeds several distance matrices in 
% a single run  
%
%
if(nargin<4) 
    error('Too few arguments!'); 
elseif(nargin<5)
  rootdir='/map/gfx0/tools/linux/src/embed/general'; 
end

for i=1:num_of_models
  D = dlmread([rootdir '/distance_matrices/' prefix int2str(i) '_' dalg '_' dtype '.txt']); 
  % distance matrices are square matrices; 
  % last column of D is superfluous due to 
  % the extra white space (before new line)  
  % created by the distance algorithm 
  m = size(D,1); 
  D=D(1:m,1:m); 
  C = embed_distance(D, 3, 'spectral'); 

  % save the color 
  dlmwrite([rootdir '/colors/' prefix int2str(i) '_' dalg '_' dtype '.txt'], C, 'delimiter', ' '); 
end

