#include "int2048.h"

namespace sjtu {

int2048::int2048() {
    sign = 0;
    val.clear();
    val.push_back(0);
}
int2048::int2048(long long t) {
    sign = 0;
    val.clear();
    unsigned long long temp;
    if (t < 0) {
        sign = 1;
        temp = -(unsigned long long)t;
    } else {
        temp = (unsigned long long)t;
    }
    if (temp == 0) {
        sign = 0;
        val.push_back(0);
    }
    while (temp > 0) {
        val.push_back(temp % BASE);
        temp = temp / BASE;
    }
}
int2048::int2048(const std::string &t) {
    read(t);
}
int2048::int2048(const int2048 &t) {
    sign = t.sign;
    val = t.val;
}
void int2048::read(const std::string &t) {
    sign = 0;
    val.clear();
    int ed = 0;
    if (t[0] == '+') {
        ed = 1;
    }
    if (t[0] == '-') {
        sign = 1;
        ed = 1;
    }
    for (int i = ed; i < t.size(); i++) {
        if (t[i] != '0') {
            ed = i;
            break;
        }
        if (i == t.size() - 1) {
            val.push_back(0);
            sign = 0;
            return ;
        }
    }
    for (int i = t.size() - 1; i >= ed; i -= DIGIT) {
        int temp = 0;
        for (int j = std::max(ed, i - DIGIT + 1); j <= i ; j++) {
            temp = temp * 10 + t[j] - '0';
        }
        val.push_back(temp);
    }
    if (val.size() == 0) {
        val.push_back(0);
    }
    if (val.size() == 1 && val[0] == 0) {
        sign = 0;
    }
}
void int2048::print() {
    if (sign) {
        if (!(val.size() == 1 && val[0] == 0)) {
            std::cout << "-";
        }
    }
    for (int i = val.size() - 1; i >= 0; i--) {
        if (i != val.size() - 1) {
            for (int j = BASE / 10; j >= 10; j /= 10) {
                if (val[i] / j == 0) {
                    std::cout << "0";
                }
            }
        }
        std::cout << val[i];
    }
}
int2048 &int2048::add(const int2048 &t) {
    if (sign && t.sign || !sign && !t.sign) {
        int l = val.size(), r = t.val.size();
        for (int i = 0; i < std::min(l, r); i++) {
            val[i] += t.val[i];
        }
        if (l < r) {
            for (int i = l; i < r; i++) {
                val.push_back(t.val[i]);
            }
            l = r;
        }
        int carry = 0;
        for (int i = 0; i < l; i++) {
            val[i] += carry;
            carry = val[i] / BASE;
            val[i] %= BASE;
        }
        if (carry != 0) {
            val.push_back(carry);
        }
        if (val.size() == 1 && val[0] == 0) {
            sign = 0;
        }
        return *this;
    } else {
        this -> sign ^= 1;
        this -> minus(t);
        this -> sign ^= 1;
        if (val.size() == 1 && val[0] == 0) {
            sign = 0;
        }
        return *this;
    }
}
int2048 add(int2048 x, const int2048 &y) {
    return x.add(y);
}
bool int2048::compare(const int2048 &t) const {
    if (t.val.size() == val.size()) {
        for (int i = val.size() - 1; i >= 0; i--) {
            if (t.val[i] != val[i]) {
                return val[i] < t.val[i];
            }
        }
        return false;
    } else {
        return val.size() < t.val.size();
    }
}
int2048 &int2048::minus(const int2048 &t) {
    if (t.sign != sign) {
        this -> sign ^= 1;
        this -> add(t);
        this -> sign ^= 1;
        if (val.size() == 1 && val[0] == 0) {
            sign = 0;
        }
        return *this;
    }
    int2048 x, y;
    if (compare(t)) {
        x = t;
        y = *this;
        x.sign ^= 1;
        y.sign ^= 1;
    } else {
        x = *this;
        y = t;
    }
    for (int i = 0; i < y.val.size(); i++) {
        x.val[i] -= y.val[i];
    }
    for (int i = 0; i < x.val.size(); i++) {
        if (x.val[i] < 0) {
            x.val[i] += BASE;
            x.val[i + 1]--;
        }
    }
    *this = x;
    int cnt = 0;
    for (int i = val.size() - 1; i >= 0; i--) {
        if (val[i] == 0) {
            cnt++;
        } else {
            break;
        }
    }
    if (cnt == val.size()) {
        cnt--;
    }
    val.resize(val.size() - cnt);
    if(val.size() == 1 && val[0] == 0) {
        sign = 0;
    }
    return *this;
}
int2048 minus(int2048 x, const int2048 &y) {
    return x.minus(y);
}
int2048 &int2048::operator=(const int2048 &t) {
    sign = t.sign;
    val = t.val;
    return *this;
}
int2048 int2048::operator+() const {
    return *this;
}
int2048 int2048::operator-() const {
    int2048 res = *this;
    if(!(res.val.size() == 1 && res.val[0] == 0)) {
        res.sign ^= 1;
    }
    return res;
}
int2048 &int2048::operator+=(const int2048 &t) {
    return add(t);
}
int2048 &int2048::operator-=(const int2048 &t) {
    return minus(t);
}
int2048 operator+(int2048 x, const int2048 &y) {
    return x.add(y);
}
int2048 operator-(int2048 x, const int2048 &y) {
    return x.minus(y);
}
bool operator==(const int2048 &x, const int2048 &y) {
    if (x.sign != y.sign) {
        return false;
    }
    if (x.val.size() != y.val.size() ) {
        return false;
    }
    for (int i = 0; i < x.val.size(); i++) {
        if (x.val[i] != y.val[i]) {
            return false;
        }
    }
    return true;
}
bool operator!=(const int2048 &x, const int2048 &y) {
    return !(x == y);
}
bool operator<(const int2048 &x, const int2048 &y) {
    if (x.sign == 1 && y.sign == 0) {
        return true;
    }
    if (x.sign == 0 && y.sign == 1) {
        return false;
    }
    if (x.sign == 1 && y.sign == 1) {
        return y.compare(x);
    }
    return x.compare(y);
}
bool operator>(const int2048 &x, const int2048 &y) {
    return (x != y) && (!(x < y));
}
bool operator<=(const int2048 &x, const int2048 &y) {
    return !(x > y);
}
bool operator>=(const int2048 &x, const int2048 &y) {
    return !(x < y);
}
std::istream &operator>>(std::istream &is, int2048 &t) {
    std::string s;
    is >> s;
    t.read(s);
    return is;
}

std::ostream &operator<<(std::ostream &os, const int2048 &t) {
    if (t.sign) {
        if (!(t.val.size() == 1 && t.val[0] == 0)) {
            os << "-";
        }
    }
    for (int i = t.val.size() - 1; i >= 0; i--) {
        if (i != t.val.size() - 1) {
            for (int j = BASE / 10; j >= 10; j /= 10) {
                if (t.val[i] / j == 0) {
                    os << "0";
                }
            }
        }
        os << t.val[i];
    }
    return os;
}
void fft(std::complex<long double> *a, int n, int flag, int *rev) {
    for (int i = 0; i < n; i++) {
        if (i < rev[i]) {
            std::swap(a[i], a[rev[i]]);
        }
    }
    for (int h = 1; h < n; h *= 2) {
        std::complex<long double> w_n = std::exp(std::complex<long double>(0, flag * PI / h));
        for (int j = 0; j < n; j += h * 2) {
            std::complex<long double> w(1, 0);
            for (int k = j; k < j + h; k++) {
                std::complex<long double> x = a[k];
                std::complex<long double> y = w * a[k + h];
                a[k] = x + y;
                a[k + h] = x - y;
                w *= w_n;
            }
        }
    }
    if (flag == -1) {
        for (int i = 0; i < n; i++) {
            a[i] /= n;
        }
    }
}

int2048 &int2048::operator*=(const int2048 &t) {
    int l_this = val.size(), l_t = t.val.size();
    int need = val.size() + t.val.size() - 1;

    int n_c = 1, n = 2;
    while ((1 << n_c) < l_this + l_t - 1) {
        n_c++;
        n <<= 1;
    }
    std::vector<std::complex<long double>> a(n), b(n);
    for (int i = 0; i < val.size(); i++) {
        a[i] = (long double)val[i];
    }
    for (int i = val.size(); i < n; i++) {
        a[i] = 0;
    }
    for (int i = 0; i < t.val.size(); i++) {
        b[i] = (long double)t.val[i];
    }
    for (int i = t.val.size(); i < n; i++) {
        b[i] = 0;
    }
    std::vector<int> rev(n);
    rev[0] = 0;
    for (int i = 0; i < n; i++) {
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (n_c - 1));
    }
    fft(a.data(), n, 1, rev.data());
    fft(b.data(), n, 1, rev.data());
    for (int i = 0; i < n; i++) {
        a[i] *= b[i];
    }
    fft(a.data(), n, -1, rev.data());
    val.resize(need + 2);
    std::vector<long long> res(need + 2, 0);
    for (int i = 0; i < need; i++) {
        res[i] = (long long)(a[i].real() + 0.5L);
    }
    for (int i = 0; i <= need; i++) {
        res[i + 1] += res[i] / BASE;
        res[i] %= BASE;
    }
    for (int i = 0; i <= need + 1; i++) {
        val[i] = res[i];
    }
    int cnt = 0;
    for (int i = need + 1; i >= 0; i--) {
        if (val[i] == 0) {
            cnt++;
        } else {
            break;
        }
    }
    if (cnt == need + 2) {
        cnt = need + 1;
    }
    val.resize(need + 2 - cnt);
    sign = sign ^ t.sign;
    if (val.size() == 1 && val[0] == 0) {
        sign = 0;
    }
    return *this;
}
int2048 operator*(int2048 x, const int2048 &y) {
    return x *= y;
}
int2048 shl_shr(int pos, int2048 t) {//正左移，负右移
    if (t.val.size() == 1 && t.val[0] == 0) {
        return t;
    }
    if (pos >= 0) {
        int n = t.val.size();
        t.val.resize(n + pos);
        for (int i = n - 1; i >= 0; i--) {
            t.val[i + pos] = t.val[i];
        }
        for (int i = 0; i < pos; i++) {
            t.val[i] = 0;
        }
    } else {
        pos = -pos;
        int n = t.val.size();
        if (pos >= n) {
            t.val.resize(1);
            t.val[0] = 0;
            t.sign = 0;
            return t;
        }
        for (int i = pos; i < n; i++) {
            t.val[i - pos] = t.val[i];
        }
        t.val.resize(n - pos);
    }
    return t;
}
int2048 div_small(const int2048 x, const int2048 y) {//倍增暴力除法
    int2048 a = y, b = int2048(1);
    std::vector<int2048> st_y, st_cnt;
    int cnt1 = 0, cnt2 = 0;
    while (a <= x) {
        st_y.push_back(a);
        st_cnt.push_back(b);
        ++cnt1;
        ++cnt2;
        b = b + b;
        a = a + a;
    }
    a = x;
    b = int2048();
    cnt1--;
    cnt2--;
    while (cnt1 >= 0) {
        if (a >= st_y[cnt1]) {
            a -= st_y[cnt1];
            b += st_cnt[cnt2];
        }
        cnt1--;
        cnt2--;
    }
    return b;
}
int2048 int2048::inv() {//递归牛顿迭代求逆
    if(val.size() <= 10) {
        int2048 temp = 1;
        temp = shl_shr(val.size() * 2, temp);
        return div_small(temp, *this);
    }
    int len = val.size();
    int k = (len + 1) / 2 + 2;
    int2048 temp = shl_shr(-(len - k), *this);
    int2048 res_temp = temp.inv();
    int2048 res = shl_shr((len - k), 2 * res_temp) - shl_shr(-2 * k, *this * res_temp *res_temp);
    return res;
}
int2048 &int2048::operator/=(const int2048 &t) {
    int sign_new = sign ^ t.sign;
    this->sign = 0;
    if (this->compare(t)) {
        if (sign_new && *this != 0) {
            return *this = int2048(-1);
        }
        return *this = int2048();
    }
    int len_this = val.size(), len_t = t.val.size();
    int2048 x = *this, y = t, temp_t = t;
    y.sign = 0;
    temp_t.sign = 0;
    if (len_this >= len_t * 2) {
        x = shl_shr(len_this - len_t * 2, x);
        y = shl_shr(len_this - len_t * 2, y);
        len_t = len_this - len_t;
        len_this = len_t * 2;
    }
    y = y.inv();
    x *= y;
    x = shl_shr(-(len_t * 2), x);
    int2048 rem = *this - x * temp_t;
    while (rem >= (temp_t)) {
        x += 1;
        rem = rem - temp_t;
    }
    while (rem.sign) {
        x -= 1;
        rem = rem + temp_t;
    }
    if (sign_new) {
        if (x * temp_t != *this) {
            x.sign = sign_new;
            x -= 1;
        }
    }
    *this = x;
    this -> sign = sign_new;
    if (val.size() == 1 && val[0] == 0) {
        this -> sign = 0;
    }
    return *this;
}
int2048 operator/(int2048 x, const int2048 &y) {
    return x /= y;
}
int2048 &int2048::operator%=(const int2048 &t) {
    *this = *this - (*this / t) * t;
    return *this;
}
int2048 operator%(int2048 x, const int2048 &y) {
    return x %= y;
}
} // namespace sjtu