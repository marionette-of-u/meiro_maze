#include <boost/dynamic_bitset.hpp>
#include "bmp.hpp"
#include <vector>
#include <random>
#include <memory>
#include <utility>
#include <string>
#include <ctime>

#define left (1)
#define right (1 << 1)
#define up (1 << 2)
#define down (1 << 3)
#define coord(x, y) ((x) + x_max * (y))

int count_flag(char a){
    int ret = 0;
    if((a & left) != 0){ ++ret; }
    if((a & right) != 0){ ++ret; }
    if((a & up) != 0){ ++ret; }
    if((a & down) != 0){ ++ret; }
    return ret;
}

struct point{
    int x, y;
};

struct path{
    int x, y;
};

void meiro(const int x_max, const int y_max){
    tty::bmp bmp(tty::bmp::ColorNum::b8, x_max * 2 + 1, y_max * 2 + 1);
    bmp.setplt(0, tty::bmp::rgb(0xFF, 0xFF, 0xFF));
    bmp.setplt(1, tty::bmp::rgb(0x00, 0x00, 0x00));

    std::mt19937 rnd(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<path> soltion_path, dummy_path;
    std::unique_ptr<char> map_instance(new char[x_max * y_max]);
    char *map = map_instance.get();
    boost::dynamic_bitset<> pass(x_max * y_max, 0), subpass(x_max * y_max, 0);
 
    auto put = [&](std::string n){
        for(int y = 0; y < bmp.height(); ++y){
            for(int x = 0; x < bmp.width(); ++x){
                bmp.pltidx(x, y, 0);
            }
        }

        for(int y = 0; y < y_max; ++y){
            for(int x = 0; x < x_max; ++x){
                char a = map[coord(x, y)];
                if(a == 0){ continue; }
                bmp.pltidx(1 + x * 2, 1 + y * 2, 1);
                if((a & left) != 0){
                    bmp.pltidx(1 + x * 2 + 1, 1 + y * 2, 1);
                }
                if((a & right) != 0){
                    bmp.pltidx(1 + x * 2 - 1, 1 + y * 2, 1);
                }
                if((a & up) != 0){
                    bmp.pltidx(1 + x * 2, 1 + y * 2 + 1, 1);
                }
                if((a & down) != 0){
                    bmp.pltidx(1 + x * 2, 1 + y * 2 - 1, 1);
                }
            }
        }

        bmp.pltidx(bmp.width() - 2, bmp.height() - 1, 1);

        bmp.write(n.c_str());
    };

    for(int i = 0; i < x_max * y_max; ++i){
        map[i] = 0;
    }

    {
        path init;
        init.x = 0, init.y = 0;
        map[0] |= down;
        pass[0] = 1;
        soltion_path.push_back(init);
    }

    auto check_sol = [&](int *a, path p){
        int an = 0;

        bool lhs = p.y > 0 && pass[coord(p.x, p.y - 1)] == 0, rhs = p.y < y_max - 1 && pass[coord(p.x, p.y + 1)] == 0;
        if((!lhs && !rhs) || (lhs && !rhs) || (!lhs && rhs)){
            if(p.x - 1 >= 0    && pass[coord(p.x - 1, p.y)] == 0){ a[an++] = left;  }
            if(p.x + 1 < x_max && pass[coord(p.x + 1, p.y)] == 0){ a[an++] = right; }
        }

        lhs = p.x > 0 && pass[coord(p.x - 1, p.y)] == 0, rhs = p.x < x_max - 1 && pass[coord(p.x + 1, p.y)] == 0;
        if((!lhs && !rhs) || (lhs && !rhs) || (!lhs && rhs)){
            if(p.y - 1 >= 0    && pass[coord(p.x, p.y - 1)] == 0){ a[an++] = up;   }
            if(p.y + 1 < y_max && pass[coord(p.x, p.y + 1)] == 0){ a[an++] = down; }
        }

        return an;
    };

    auto check_comm = [&](int *a, path p){
        auto isolation = [&](int x, int y){
            return
                (x - 1 >= 0    ? subpass[coord(x - 1, y)] == 0 : true) &&
                (x + 1 < x_max ? subpass[coord(x + 1, y)] == 0 : true) &&
                (y - 1 >= 0    ? subpass[coord(x, y - 1)] == 0 : true) &&
                (y + 1 < y_max ? subpass[coord(x, y + 1)] == 0 : true);
        };

        int an = 0;

        bool lhs = p.y > 0 && subpass[coord(p.x, p.y - 1)] == 0, rhs = p.y < y_max - 1 && subpass[coord(p.x, p.y + 1)] == 0;
        if((!lhs && !rhs) || (lhs && !rhs) || (!lhs && rhs)){
            if(p.x - 1 >= 0    && subpass[coord(p.x - 1, p.y)] == 0 && isolation(p.x - 1, p.y)){ a[an++] = left;  }
            if(p.x + 1 < x_max && subpass[coord(p.x + 1, p.y)] == 0 && isolation(p.x + 1, p.y)){ a[an++] = right; }
        }

        lhs = p.x > 0 && subpass[coord(p.x - 1, p.y)] == 0, rhs = p.x < x_max - 1 && subpass[coord(p.x + 1, p.y)] == 0;
        if((!lhs && !rhs) || (lhs && !rhs) || (!lhs && rhs)){
            if(p.y - 1 >= 0    && subpass[coord(p.x, p.y - 1)] == 0 && isolation(p.x, p.y - 1)){ a[an++] = up;   }
            if(p.y + 1 < y_max && subpass[coord(p.x, p.y + 1)] == 0 && isolation(p.x, p.y + 1)){ a[an++] = down; }
        }

        return an;
    };

    auto sol = [&](path p){
        int a[4] = { 0 };
        int an = check_sol(a, p);

        if(an == 0){ return false; }

        path q = p;
        char im = 0, to = static_cast<char>(a[static_cast<int>(rnd() % an)]);
        
        switch(to){
        case left:
            --p.x;
            im = right;
            break;

        case right:
            ++p.x;
            im = left;
            break;

        case up:
            --p.y;
            im = down;
            break;

        case down:
            ++p.y;
            im = up;
            break;
        }

        map[coord(q.x, q.y)] |= im;
        map[coord(p.x, p.y)] |= to;
        pass[coord(p.x, p.y)] = 1;
        soltion_path.push_back(p);

        return true;
    };

    auto comm = [&](path &p){
        int a[4] = { 0 };
        int an = check_comm(a, p);

        if(an == 0){
            return false;
        }
        
        path q = p;
        char im = 0, to = static_cast<char>(a[static_cast<int>(rnd() % an)]);

        switch(to){
        case left:
            --p.x;
            im = right;
            break;

        case right:
            ++p.x;
            im = left;
            break;

        case up:
            --p.y;
            im = down;
            break;

        case down:
            ++p.y;
            im = up;
            break;
        }

        map[coord(q.x, q.y)] |= im;
        map[coord(p.x, p.y)] |= to;
        subpass[coord(p.x, p.y)] = 1;
        dummy_path.push_back(p);

        return true;
    };

    for(; ; ){
        if(!sol(soltion_path.back())){
            path p = soltion_path.back();
            char a = map[coord(p.x, p.y)];
            map[coord(p.x, p.y)] = 0;
            soltion_path.pop_back();
            path q = soltion_path.back();

            if((a & left) != 0){
                map[coord(q.x, q.y)] &= ~right & ((1 << 4) - 1);
            }else if((a & right) != 0){
                map[coord(q.x, q.y)] &= ~left & ((1 << 4) - 1);
            }else if((a & up) != 0){
                map[coord(q.x, q.y)] &= ~down & ((1 << 4) - 1);
            }else if((a & down) != 0){
                map[coord(q.x, q.y)] &= ~up & ((1 << 4) - 1);
            }

            continue;
        }

        if(soltion_path.back().x == x_max - 1 && soltion_path.back().y == y_max - 1){
            break;
        }
    }

    pass.reset();
    for(auto iter = soltion_path.begin(); iter != soltion_path.end(); ++iter){
        pass[coord(iter->x, iter->y)] = 1;
    }
    soltion_path.swap(decltype(soltion_path)());

    put("ans.bmp");

    std::vector<path> rest;
    rest.reserve(x_max * y_max - soltion_path.size() + 4);
    for(int y = 0; y < y_max; ++y){
        for(int x = 0; x < x_max; ++x){
            if(map[coord(x, y)] != 0){ continue; }
            path p;
            p.x = x, p.y = y;
            rest.push_back(p);
        }
    }

    for(int i = 0; i < x_max * y_max; ++i){
        std::swap(rest[rnd() % rest.size()], rest[rnd() % rest.size()]);
    }

    for(auto iter = rest.begin(); iter != rest.end(); ++iter){
        int x = iter->x, y = iter->y;
        if(pass[coord(x, y)] != 0){ continue; }

        path p;
        p.x = x, p.y = y;
        subpass.reset();
        dummy_path.clear();
        subpass[coord(p.x, p.y)] = 1;
        dummy_path.push_back(p);
        for(; ; ){
            if(!comm(p)){
                if(count_flag(map[coord(p.x, p.y)]) == 3){
                    subpass[coord(p.x, p.y)] = 1;
                    dummy_path.push_back(p);

                    break;
                }

                int a[4] = { 0 };
                int an = 0;
                if(p.x - 1 >= 0    && subpass[coord(p.x - 1, p.y)] == 0 && count_flag(map[coord(p.x - 1, p.y)]) < 3){
                    a[an++] = left;
                }
                if(p.x + 1 < x_max && subpass[coord(p.x + 1, p.y)] == 0 && count_flag(map[coord(p.x + 1, p.y)]) < 3){
                    a[an++] = right;
                }
                if(p.y - 1 >= 0    && subpass[coord(p.x, p.y - 1)] == 0 && count_flag(map[coord(p.x, p.y - 1)]) < 3){
                    a[an++] = up;
                }
                if(p.y + 1 < y_max && subpass[coord(p.x, p.y + 1)] == 0 && count_flag(map[coord(p.x, p.y + 1)]) < 3){
                    a[an++] = down;
                }

                if(an == 0){
                    path r = dummy_path.back();
                    char a = map[coord(r.x, r.y)];
                    map[coord(r.x, r.y)] = 0;
                    dummy_path.pop_back();
                    p = dummy_path.back();

                    if((a & left) != 0){
                        map[coord(p.x, p.y)] &= ~right & ((1 << 4) - 1);
                    }
                    if((a & right) != 0){
                        map[coord(p.x, p.y)] &= ~left & ((1 << 4) - 1);
                    }
                    if((a & up) != 0){
                        map[coord(p.x, p.y)] &= ~down & ((1 << 4) - 1);
                    }
                    if((a & down) != 0){
                        map[coord(p.x, p.y)] &= ~up & ((1 << 4) - 1);
                    }
                    continue;
                }

                path q = p;
                char im = 0, to = static_cast<char>(a[static_cast<int>(rnd() % an)]);
                switch(to){
                case left:
                    --p.x;
                    im = right;
                    break;

                case right:
                    ++p.x;
                    im = left;
                    break;

                case up:
                    --p.y;
                    im = down;
                    break;

                case down:
                    ++p.y;
                    im = up;
                    break;
                }

                map[coord(q.x, q.y)] |= im;
                map[coord(p.x, p.y)] |= to;
                subpass[coord(p.x, p.y)] = 1;
                dummy_path.push_back(p);
                if(!pass[coord(p.x, p.y)]){
                    continue;
                }else{ break; }
            }
        }

        for(auto iter = dummy_path.begin(); iter != dummy_path.end(); ++iter){
            pass[coord(iter->x, iter->y)] = 1;
        }
    }

    put("sample.bmp");
}

int main(){
    meiro(1000, 1000);

    return 0;
}

