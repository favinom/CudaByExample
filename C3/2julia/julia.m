function it=julia

N=5001;
MAX_IT=10000;

e=1;

c_re=-0.8;
c_im=0.156;

r=2;
r2=r*r;

x=linspace(-e,e,N);
y=linspace(-e,e,N);

[re,im]=ndgrid(x,y);

re=re(:);
im=im(:);

it=(MAX_IT+1)*ones(length(re),1);

re2=re.^2;
im2=im.^2;

for i=0:MAX_IT-1
    
    im=2*re.*im+c_im;
    re=re2-im2+c_re;

    re2=re.^2;
    im2=im.^2;

    it(re2+im2>r2 & it==MAX_IT+1)=i;
end

it = reshape(it, N, N);
imagesc(it);
axis image;
colorbar;
colormap(turbo);
