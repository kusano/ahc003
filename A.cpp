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

        sy = vector<int>(1000);
        sx = vector<int>(1000);
        ty = vector<int>(1000);
        tx = vector<int>(1000);
        a = vector<int>(1000);
        e = vector<double>(1000);
        for (int k=0; k<1000; k++)
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

    vector<int> DH(H*2-1, 1000);
    vector<int> DV(W*2-1, 1000);

    for (int k=0; k<1000; k++)
    {
        //if (k%100==99)
        //{
        //    for (int y=0; y<H; y++)
        //    {
        //        for (int x=0; x<W-1; x++)
        //            cerr<<(x==0?"":" ")<<HV[2*y][2*x+1];
        //        cerr<<endl;
        //    }
        //    for (int y=0; y<H-1; y++)
        //    {
        //        for (int x=0; x<W; x++)
        //            cerr<<(x==0?"":" ")<<HV[2*y+1][2*x];
        //        cerr<<endl;
        //    }
        //}

        int sy, sx, ty, tx;
        tester.getInput(&sy, &sx, &ty, &tx);
        sx *= 2;
        sy *= 2;
        tx *= 2;
        ty *= 2;

        int oo = 99999999;
        vector<vector<int>> D(H*2-1, vector<int>(W*2-1, oo));
        vector<vector<char>> M(H*2-1, vector<char>(W*2-1));
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
                    int td = D[fy][fx] + (dx[d]!=0 ? DH[fy]/1024 : DV[fx]/1024);
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

        //  correct weight
        int diff = (result-D[ty][tx])/(int)path.size()*1024/29;
        x = sx;
        y = sy;
        for (int d: path)
        {
            if (dx[d]!=0)
                DH[y] = max(1000*1024, min(9000*1024, DH[y]+diff));
            else
                DV[x] = max(1000*1024, min(9000*1024, DV[x]+diff));
            x += dx[d]*2;
            y += dy[d]*2;
        }
    }

    tester.printScore();
}
