function genetrconf(C, y, r, f)
scatter(y(1,:), y(2,:),r,[1 1 1], 'filled'); %, 'MarkerEdge',[0.5 0.5 0.5]); 
if(f)
    for i=1:603
        text(y(1,i), y(2,i), int2str(i), 'FontSize', 8, 'Color', C(i,:));%,'FontWeight', 'bold');
    end
end