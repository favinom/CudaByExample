function stats = mytimeit(f, n_warmup, n_runs)
    if nargin < 2
        n_warmup = 2;
    end
    if nargin < 3
        n_runs = 15;
    end

    for k = 1:n_warmup
        f();
    end

    t = zeros(n_runs,1);
    for k = 1:n_runs
        tic;
        f();
        t(k) = toc;
    end

    stats.times  = t;
    stats.median = median(t);
    stats.mean   = mean(t);
    stats.min    = min(t);
    stats.max    = max(t);
    stats.std    = std(t);
end