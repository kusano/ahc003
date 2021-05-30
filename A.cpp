#ifndef PARAM_HV_MIN
#define PARAM_HV_MIN 0.
#endif
#ifndef PARAM_HV_MAX
#define PARAM_HV_MAX 8600.
#endif
#ifndef PARAM_HV_INIT
#define PARAM_HV_INIT 2800.
#endif
#ifndef PARAM_DELTA_MIN
#define PARAM_DELTA_MIN -250.
#endif
#ifndef PARAM_DELTA_MAX
#define PARAM_DELTA_MAX 10000.
#endif
#ifndef PARAM_DELTA_INIT
#define PARAM_DELTA_INIT 2500.
#endif
#ifndef PARAM_DELTA_RATE
#define PARAM_DELTA_RATE 0.13
#endif

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>
#include <cassert>
#include <cmath>
using namespace std;

const int W = 30;
const int H = 30;
const int K = 1000;

#ifdef ONLINE_JUDGE

class Tester
{
public:
    void init()
    {
    }

    void getInput(int *sy, int *sx, int *ty, int *tx)
    {
        cin>>*sy>>*sx>>*ty>>*tx;
    }

    int answer(string path)
    {
        cout<<path<<endl;
        int result;
        cin>>result;
        return result;
    }

    void printScore()
    {
    }
};

#else

class Tester
{
    vector<vector<int>> h, v;
    vector<int> sy, sx, ty, tx, a;
    vector<double> e;
    int k;
    double score;

public:
    void init()
    {
        h = vector<vector<int>>(H, vector<int>(W-1));
        for (int y=0; y<H; y++)
            for (int x=0; x<W-1; x++)
                cin>>h[y][x];
        v = vector<vector<int>>(H-1, vector<int>(W));
        for (int y=0; y<H-1; y++)
            for (int x=0; x<W; x++)
                cin>>v[y][x];

        sy = vector<int>(K);
        sx = vector<int>(K);
        ty = vector<int>(K);
        tx = vector<int>(K);
        a = vector<int>(K);
        e = vector<double>(K);
        for (int k=0; k<K; k++)
            cin>>sy[k]>>sx[k]>>ty[k]>>tx[k]>>a[k]>>e[k];

        k = 0;
        score = 0;
    }

    void getInput(int *sy, int *sx, int *ty, int *tx)
    {
        *sy = this->sy[k];
        *sx = this->sx[k];
        *ty = this->ty[k];
        *tx = this->tx[k];
    }

    int answer(string path)
    {
        cout<<path<<endl;

        int y = sy[k];
        int x = sx[k];
        int b = 0;
        for (char c: path)
        {
            switch (c)
            {
            case 'U':
                b += v[y-1][x];
                y--;
                break;
            case 'D':
                b += v[y][x];
                y++;
                break;
            case 'L':
                b += h[y][x-1];
                x--;
                break;
            case 'R':
                b += h[y][x];
                x++;
                break;
            }
            assert(0<=x && x<W && 0<=y && y<H);
        }
        assert(x==tx[k] && y==ty[k]);
        score += pow(0.998, 1000-(k+1))*a[k]/b;
        int result = int(b*e[k]);
        k++;
        return result;
    }

    void printScore()
    {
        cerr<<"score: "<<(long long)(2312311*score)<<endl;
    }
};

#endif

int main()
{
    Tester tester;
    tester.init();

    string ds = "UDLR";
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    vector<double> HL(H*2-1, PARAM_HV_INIT), HR(H*2-1, PARAM_HV_INIT);
    vector<double> VU(W*2-1, PARAM_HV_INIT), VD(H*2-1, PARAM_HV_INIT);
    vector<vector<double>> delta(H*2-1, vector<double>(W*2-1));

    vector<int> sxs, sys;
    vector<vector<int>> paths;
    vector<int> results;

    auto dist = [&](int fx, int fy, int dir) -> int
    {
        if (dx[dir]!=0)
        {
            int mx = fx+dx[dir];
            return max(1, int((HL[fy]*((2*W-3)-mx) + HR[fy]*(mx-1))/(2*W-4) + delta[fy][mx]));
        }
        else
        {
            int my = fy+dy[dir];
            return max(1, int((VU[fx]*((2*H-3)-my) + VD[fx]*(my-1))/(2*H-4) + delta[my][fx]));
        }
    };

    auto clip= [&](double v, double mn, double mx) -> double
    {
        return min(mx, max(mn, v));
    };

    for (int k=0; k<K; k++)
    {
        int sy, sx, ty, tx;
        tester.getInput(&sy, &sx, &ty, &tx);
        sx *= 2;
        sy *= 2;
        tx *= 2;
        ty *= 2;

        sxs.push_back(sx);
        sys.push_back(sy);

        int oo = 99999999;
        vector<vector<int>> D(H*2-1, vector<int>(W*2-1, oo));
        vector<vector<int>> M(H*2-1, vector<int>(W*2-1));
        priority_queue<pair<int, pair<int, int>>> Q;
        vector<vector<bool>> F(H*2-1, vector<bool>(W*2-1));

        D[sy][sx] = 0;
        Q.push({0, {sx, sy}});

        while (!Q.empty())
        {
            int fx = Q.top().second.first;
            int fy = Q.top().second.second;
            Q.pop();
            if (F[fy][fx])
                continue;
            F[fy][fx] = true;

            for (int d=0; d<4; d++)
            {
                int tx = fx+dx[d]*2;
                int ty = fy+dy[d]*2;

                if (0<=tx && tx<2*W &&
                    0<=ty && ty<2*H)
                {
                    int td = D[fy][fx] + dist(fx, fy, d);
                    if (td<D[ty][tx])
                    {
                        D[ty][tx] = td;
                        M[ty][tx] = d;
                        Q.push({-td, {tx, ty}});
                    }
                }
            }
        }

        int x = tx;
        int y = ty;
        vector<int> path;
        while (!(x==sx && y==sy))
        {
            int d = M[y][x];
            path.push_back(d);
            x -= dx[d]*2;
            y -= dy[d]*2;
        }
        reverse(path.begin(), path.end());

        string ans;
        for (int p: path)
            ans += ds[p];

        int result = tester.answer(ans);

        paths.push_back(path);
        results.push_back(result);

        //  correct distances
        for (int k2=0; k2<=k; k2++)
        {
            int x = sxs[k2];
            int y = sys[k2];
            int d = 0;
            for (int p: paths[k2])
            {
                d += dist(x, y, p);
                x += dx[p]*2;
                y += dy[p]*2;
            }
            double diff = double(results[k2]-d)/paths[k2].size();
            x = sxs[k2];
            y = sys[k2];
            for (int p: paths[k2])
            {
                if (dx[p]!=0)
                {
                    int mx = x+dx[p];
                    double t = diff*(1-PARAM_DELTA_RATE)*(2*W-4)/(((2*W-3)-mx)*((2*W-3)-mx)+(mx-1)*(mx-1))/(W-1);
                    HL[y] = clip(HL[y]+((2*W-3)-mx)*t, PARAM_HV_MIN, PARAM_HV_MAX);
                    HR[y] = clip(HR[y]+(mx-1)*t, PARAM_HV_MIN, PARAM_HV_MAX);
                    delta[y][mx] = clip(delta[y][mx]+diff*PARAM_DELTA_RATE/(W-1), PARAM_DELTA_MIN, PARAM_DELTA_MAX);
                }
                else
                {
                    int my = y+dy[p];
                    double t = diff*(1-PARAM_DELTA_RATE)*(2*H-4)/(((2*H-3)-my)*((2*H-3)-my)+(my-1)*(my-1))/(H-1);
                    VU[x] = clip(VU[x]+((2*H-3)-my)*t, PARAM_HV_MIN, PARAM_HV_MAX);
                    VD[x] = clip(VD[x]+(my-1)*t, PARAM_HV_MIN, PARAM_HV_MAX);
                    delta[my][x] = clip(delta[my][x]+diff*PARAM_DELTA_RATE/(H-1), PARAM_DELTA_MIN, PARAM_DELTA_MAX);
                }
                x += dx[p]*2;
                y += dy[p]*2;
            }
        }
    }
    tester.printScore();
}
