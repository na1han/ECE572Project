function [ p ] = displayTable( img )
imshow(uint8(img))
p = get(gca, 'CurrentPoint');
p = [p(1,1) p(1,2)];
end