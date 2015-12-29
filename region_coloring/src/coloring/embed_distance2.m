function [C y] = embed_distance(M, d, method, opts)
%
% embed_distance - Given D-dimensional X1 ... XN points
% and an N by N distance matrix A,  it creates corresponding
% d-dimensional points Y1...YN where d << D.

% INPUT
%  M      : distance matrix
%  d      : target dimension (note the embedding function f : R^D-->R^d)
%  method : embedding method
%  opts   : options
%
% OUTPUT
%  C: N by 3 matrix containing color values for X1...XN obtained using
%     L*a*b* color space
%  y: embedding  coordinates
%

if(nargin < 4)
    opts.dorefine=1;
    opts.refine.ratio =0.1;
    opts.refine.numiter = 0;
    opts.refine.R = 0.1* std_for_dmatrix(M);
    opts.method=method;
    %opts.method='spectral';
    %opts.method='mds';
    %opts.method='isomap';
    opts.coloring='lab';
    opts.scale=1;
    opts.bbox = [-50 -20; 70 70]; % in lab
    opts.bboxsize= [120 90]; % h w d
end

%if(opts.outliercap==1)
% linearize distances and find standard deviation
% and cap distances to mu+2*sigma
%end

if(strcmp(opts.method,'mds'))
    % embed w/ one of the standard methods and
    % and refine afterwards
    Y  = (mdscale(M,d))'; % non-metric mds. comment this line,
    % uncomment the following line for metric scaling.

    % Y = mdscale(M,d,'criterion','metricstress');
elseif(strcmp(opts.method, 'isomap'))
    %
    options.dim = 1:d;
    options.display=0;
    %Ycell = Isomap(M, 'epsilon', 4*opts.refine.R);
    Ycell = IsomapII(M, 'k', 10, options); % see IsomapII to get sense of
    % these options
    for i=1:d
        Y(i,:)= Ycell.coords{d}(i,:);
    end

elseif(strcmp(opts.method,'lle'))
    %Y = lle();
elseif(strcmp(opts.method,'spectral'))
    Y = spectral(M, d);
    Y = Y';
elseif(strcmp(opts.method,'iterative'))
    %
end

% refine
if(opts.dorefine==1)
    disp('refining...')
    Y = dist_refine_local(M, Y, opts.refine.R, opts.refine.ratio, opts.refine.numiter);
end

y = Y;
% embedding is done
if(opts.scale==1)

    % uniform scale to coord 'box'
    max_Y  = max(Y, [], 2);
    min_Y  = min(Y, [], 2);
    size_Y = max_Y - min_Y;

    [ignore, I] = sort(opts.bboxsize, 'descend');
    [ignore, J] = sort(size_Y, 'descend');

    N = size(M,1);
    Y = Y - repmat(min_Y, 1,N);
    Y = Y./size_Y(J(1));


    Y(J(1),:) = Y(J(1),:)*opts.bboxsize(I(1))+0.5*(opts.bbox(1,I(1)) + opts.bbox(1,I(1)));
    Y(J(2),:) = Y(J(2),:)*opts.bboxsize(I(2))+0.5*(opts.bbox(1,I(2)) + opts.bbox(1,I(2)));
    %   Y(J(3),:) = Y(J(3),:)*opts.bboxsize(I(3))+0.5*(opts.bbox(1,I(3)) + opts.bbox(1,I(3)));

end
y = Y;

C=[];

%color
if(strcmp(opts.coloring,'lab'))

    a = Y(J((I==1)),:);
    b = Y(J((I==2)),:);
    l = ones(size(a))*70; % constant l
    C = applycform([l' a' b'], makecform('lab2srgb'));

elseif(opts.coloring == 'rgb')
end

