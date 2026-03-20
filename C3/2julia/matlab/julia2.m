function it=julia2

N=5001;
MAX_IT = 200;

c_re = single(-0.8);
c_im = single(0.156);

x0=single(-1);
y0=single(-1);

r  = single(100);
r2 = r*r;

dx=single(2/(N-1));
dy=single(2/(N-1));

x = x0+dx*(0:N-1);
y = y0+dy*(0:N-1);

[re, im] = ndgrid(x, y);

re = re(:);
im = im(:);

it = uint16((MAX_IT + 1) * ones(length(re), 1));

re2 = re.^2;
im2 = im.^2;

two = single(2);

for i = 0:MAX_IT-1

    im = two * re .* im + c_im;
    re = re2 - im2 + c_re;

    re2 = re.^2;
    im2 = im.^2;

    it(re2 + im2 > r2 & it == MAX_IT + 1) = uint16(i);
end

it = reshape(it, N, N);
imagesc(it);
axis image;
colorbar;
colormap(turbo);

end