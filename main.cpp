// Ольга Лобач. ИУ8-13. Калькулятор. Решение с учетом, скобок, функций (cos, tg..) и переменной x.
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <algorithm>
#include <math.h>

bool errors(std::string v) {
    if (v.find("-+") != std::string::npos || v.find("++") != std::string::npos) {
        std::cout << "Ошибка! Неверный ввод!";
        return false;
    }
    if (std::find_if(v.begin(), v.end(), [](auto &a) { return std::isdigit(a); }) == v.end() &&
        (v.find('x') == std::string::npos)) {
        std::cout << "Ошибка! Неверный ввод!";
        return false;
    }
    if (std::count(v.begin(), v.end(), '(') != std::count(v.begin(), v.end(), ')')) {
        std::cout << "Ошибка! Неверное число скобок";
        return 0;
    }
    return true;
}

double solve(std::string v) {
    if (v.find("--") != std::string::npos) {
        v = v.replace(v.find("--"), 2, "+");
    }
    if (v.find("+-") != std::string::npos) {
        v = v.replace(v.find("+-"), 2, "-");
    }
    std::map<char, int> operations;
    char tmp_sign = 0;
    operations['*'] = 1;
    operations['/'] = 1;
    operations['+'] = 2;
    operations['-'] = 2;
    bool first_oper = false; // операция с первым приорететом
    std::string tmp_numb;
    double first_vr;
    std::list<double> numbers;
    std::list<char> signs;
    for (size_t i = 0; i < v.size(); ++i) {
        if (std::isdigit(v[i]) || (v[i] == '.') || (i == 0 && v[i] == '-')) {
            tmp_numb += v[i];
            if (v[i] == '.' || (i == 0 && v[i] == '-')) {
                continue;
            }
        }
        if (!(std::isdigit(v[i])) || (i == v.size() - 1)) {
            if (!(tmp_numb.empty())) {
                if (first_oper) {
                    switch (tmp_sign) {
                        case '*':
                            first_vr = numbers.back() * std::stod(tmp_numb);
                            break;
                        case '/':
                            first_vr = numbers.back() / std::stod(tmp_numb);
                    }
                    numbers.pop_back();
                    numbers.push_back(first_vr);
                } else {
                    numbers.push_back(std::stod(tmp_numb));
                }
                tmp_numb.clear();
                tmp_sign = v[i];
                if (operations[v[i]] == 1 || (i == v.size() - 1)) {
                    first_oper = true;
                } else {
                    first_oper = false;
                    signs.push_back(v[i]);
                }
            }
        }
    }
    if (signs.empty()) {
        return numbers.front();
    }
    double final_vr;
    int n = signs.size();
    for (size_t i = 0; i < n; ++i) {
        if (i == 0) {
            final_vr = numbers.front();
            numbers.pop_front();
        }
        switch (signs.front()) {
            case '+':
                final_vr += numbers.front();
                break;
            case '-':
                final_vr -= numbers.front();
        }
        numbers.pop_front();
        signs.pop_front();
    }
    return final_vr;
}

double solve_main(std::string vr) {
    int count_open;
    std::string tmp_res;
    std::list<size_t> index;
    if (vr.find('(') == std::string::npos && vr.find(')') == std::string::npos) {
        return solve(vr);
    }
    bool bCanExit = false;
    while (!bCanExit) {
        count_open = 0;
        for (size_t i = 0; i < vr.length(); ++i) {
            if (vr[i] == '(') {
                count_open += 1;
                index.push_back(i);
            }
            if (vr[i] == ')') {
                tmp_res = std::to_string(solve(vr.substr(index.back() + 1, i - index.back() - 1)));
                count_open -= 1;
                vr = vr.replace(index.back(), i - index.back() + 1, tmp_res);
                index.pop_back();
                if (vr.find('(') == std::string::npos) {
                    bCanExit = true;
                }
                break;
            }
        }
    }

    return solve(vr);
}

std::string trag_func(std::string &vr) {
    if ((vr.find("sin") == std::string::npos) && (vr.find("cos") == std::string::npos) &&
        (vr.find("tg") == std::string::npos) && (vr.find("ctg") == std::string::npos) &&
        (vr.find("exp") == std::string::npos)) {
        return vr;
    }
    const std::array<std::string, 5> funcs = {"sin", "cos", "tg", "ctg", "exp"};
    for (auto &f: funcs) {
        if (vr.find(f) != std::string::npos) {
            std::string tmp_str;
            std::string cut_vr = vr.substr(vr.find(f) + f.length(), vr.length());
            int count = 0;
            int k = 0;
            for (auto &elem: cut_vr) {
                if (elem == '(') {
                    count += 1;
                    k++;
                } else if (elem == ')') {
                    k++;
                    count -= 1;
                    if (count == 0) {
                        std::string inside_vr;
                        if (f == "sin") {
                            inside_vr = std::to_string(sin(solve_main(tmp_str)));
                        }
                        if (f == "cos") {
                            inside_vr = std::to_string(cos(solve_main(tmp_str)));
                        }
                        if (f == "tg") {
                            inside_vr = std::to_string(tan(solve_main(tmp_str)));
                        }
                        if (f == "ctg") {
                            inside_vr = std::to_string(1 / (tan(solve_main(tmp_str))));
                        }
                        if (f == "exp") {
                            inside_vr = std::to_string(exp(solve_main(tmp_str)));
                        }
                        vr.replace(vr.find(f), f.length() + k + tmp_str.length(), inside_vr);
                        break;

                    }
                } else {
                    tmp_str += elem;
                }
            }
        }
    }
    return vr;


}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "Error: Use one parameter!\n";
        return 0;
    }
    std::string math_expr = argv[1];
    if (!errors(math_expr)) {
        return 0;
    }
    std::string val_x;
    std::cout << "Введите x = ";
    std::cin >> val_x;
    int count_x = std::count(math_expr.begin(), math_expr.end(), 'x');
    for (size_t i = 0; i < count_x; ++i) {
        math_expr = math_expr.replace(math_expr.find('x'), 1, val_x);
    }
    try {
        math_expr = trag_func(math_expr);
    }
    catch (...) { std::cout << "В выражении обнаружен ошибка!"; }
    std::cout << solve_main(math_expr);
    return 0;
}
