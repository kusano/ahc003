#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>
#include <cassert>
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
        score = score*0.998+a[k]/b;
        int result = int(b*e[k]);
        k++;
        return result;
    }

    void printScore()
    {
        cout<<long long(2312311*score)<<endl;
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

    for (int k=0; k<1000; k++)
    {
        int sy, sx, ty, tx;
        tester.getInput(&sy, &sx, &ty, &tx);

        int oo = 99999999;
        vector<vector<int>> D(H*2-1, vector<int>(W*2-1, oo));
        vector<vector<char>> M(H*2-1, vector<char>(W*2-1));
        priority_queue<pair<int, pair<int, int>>> Q;
        vector<vector<bool>> F(H*2-1, vector<bool>(W*2-1));

        D[sy*2][sx*2] = 0;
        Q.push({0, {sx*2, sy*2}});

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
                    int td = D[fy][fx]+1;
                    if (td<D[ty][tx])
                    {
                        D[ty][tx] = td;
                        M[ty][tx] = d;
                        Q.push({-td, {tx, ty}});
                    }
                }
            }
        }

        int x = tx*2;
        int y = ty*2;
        string ans;
        while (!(x==sx*2 && y==sy*2))
        {
            int d = M[y][x];
            ans += ds[d];
            x -= dx[d]*2;
            y -= dy[d]*2;
        }
        reverse(ans.begin(), ans.end());
        tester.answer(ans);
    }
    tester.printScore();
}
