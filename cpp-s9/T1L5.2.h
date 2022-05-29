#pragma once
#include <array>
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <utility>

using namespace std;


class VehiclePlate {
private:
    auto AsTuple() const {
        return tie(letters_, digits_, region_);
    }

public:
    bool operator==(const VehiclePlate& other) const {
        return AsTuple() == other.AsTuple();
    }

    VehiclePlate(char l0, char l1, int digits, char l2, int region)
        : letters_{ l0, l1, l2 }
        , digits_(digits)
        , region_(region) {
    }

    string ToString() const {
        ostringstream out;
        out << letters_[0] << letters_[1];

        // чтобы дополнить цифровую часть номера слева нул€ми
        // до трЄх цифр, используем подобные манипул€торы:
        // setfill задаЄт символ дл€ заполнени€,
        // right задаЄт выравнивание по правому краю,
        // setw задаЄт минимальное желаемое количество знаков
        out << setfill('0') << right << setw(3) << digits_;
        out << letters_[2] << setw(2) << region_;

        return out.str();
    }

private:
    array<char, 3> letters_;
    int digits_;
    int region_;
};

ostream& operator<<(ostream& out, VehiclePlate plate) {
    out << plate.ToString();
    return out;
}

// возьмите реализацию хешера из прошлого задани€
class VehiclePlateHasher {
    hash<string> hasher_;
public:
    size_t operator()(const VehiclePlate& plate) const {
        // измените эту функцию, чтобы она учитывала все данные номера
        // рекомендуетс€ использовать метод ToString() и существующий 
        // класс hash<string>
        return static_cast<size_t>(hasher_(plate.ToString()));
    }
};

// выбросьте это исключение в случае ошибки парковки
struct ParkingException {};

template <typename Clock>
class Parking {
    // при обращении к типу внутри шаблонного параметра мы об€заны использовать 
    // typename; чтобы этого избежать, объ€вим псевдонимы дл€ нужных типов
    using Duration = typename Clock::duration;
    using TimePoint = typename Clock::time_point;

public:
    Parking(int cost_per_second)
        : cost_per_second_(cost_per_second) {
    }

    // запарковать машину с указанным номером
    void Park(VehiclePlate car) {
        if (now_parked_.count(car)) {
            throw ParkingException();
        }
        else {
            now_parked_[car] = Clock::now();
        }
    }

    // забрать машину с указанным номером
    void Withdraw(const VehiclePlate& car) {
        if (!now_parked_.count(car)) {
            throw ParkingException();
        }
        else {
            complete_parks_[car] = Clock::now() - now_parked_.at(car);
            now_parked_.erase(car);
        }
    }

    // получить счЄт за конкретный автомобиль
    int64_t GetCurrentBill(const VehiclePlate& car) const {
        Duration duration{};
        if (now_parked_.count(car) == 0 && complete_parks_.count(car) == 0) {
            return 0;
        }

        if (now_parked_.count(car) != 0) {
            duration += Clock::now() - now_parked_.find(car)->second;
        }

        if (complete_parks_.count(car) != 0) {
            duration += complete_parks_.find(car)->second;
        }

        return chrono::duration_cast<chrono::seconds>(duration).count() * cost_per_second_;
    }

    // завершить расчЄтный период
    // те машины, которые наход€тс€ на парковке на данный момент, должны 
    // остатьс€ на парковке, но отсчЄт времени дл€ них начинаетс€ с нул€
    unordered_map<VehiclePlate, int64_t, VehiclePlateHasher> EndPeriodAndGetBills() {
        unordered_map<VehiclePlate, int64_t, VehiclePlateHasher> result{};

        if (now_parked_.size() != 0) {
            auto it = now_parked_.begin();
            auto zeroing = Clock::now();
            while (it != now_parked_.end()) {
                auto bill_prev = chrono::duration_cast<chrono::seconds>(zeroing - it->second).count() * cost_per_second_;
                result[it->first] += bill_prev;
                it->second = zeroing;
                ++it;
            }
        }

        if (complete_parks_.size() != 0) {
            auto it = complete_parks_.begin();
            while (it != complete_parks_.end()) {
                auto bill_total = chrono::duration_cast<chrono::seconds>(it->second).count() * cost_per_second_;
                result[it->first] += bill_total;
                ++it;
            }
        }

        complete_parks_.clear();
        return result;
    }

    // не мен€йте этот метод
    auto& GetNowParked() const {
        return now_parked_;
    }

    // не мен€йте этот метод
    auto& GetCompleteParks() const {
        return complete_parks_;
    }

private:
    int cost_per_second_;
    unordered_map<VehiclePlate, TimePoint, VehiclePlateHasher> now_parked_;
    unordered_map<VehiclePlate, Duration, VehiclePlateHasher> complete_parks_;
};

// эти часы удобно использовать дл€ тестировани€
// они покажут столько времени, сколько вы задали €вно
class TestClock {
public:
    using time_point = chrono::system_clock::time_point;
    using duration = chrono::system_clock::duration;

    static void SetNow(int seconds) {
        current_time_ = seconds;
    }

    static time_point now() {
        return start_point_ + chrono::seconds(current_time_);
    }

private:
    inline static time_point start_point_ = chrono::system_clock::now();
    inline static int current_time_ = 0;
};

int main() {
    Parking<TestClock> parking(10);

    TestClock::SetNow(10);
    parking.Park({ 'A', 'A', 111, 'A', 99 });

    TestClock::SetNow(20);
    parking.Withdraw({ 'A', 'A', 111, 'A', 99 });
    parking.Park({ 'B', 'B', 222, 'B', 99 });

    TestClock::SetNow(40);
    assert(parking.GetCurrentBill({ 'A', 'A', 111, 'A', 99 }) == 100);
    assert(parking.GetCurrentBill({ 'B', 'B', 222, 'B', 99 }) == 200);
    parking.Park({ 'A', 'A', 111, 'A', 99 });

    TestClock::SetNow(50);
    assert(parking.GetCurrentBill({ 'A', 'A', 111, 'A', 99 }) == 200);
    assert(parking.GetCurrentBill({ 'B', 'B', 222, 'B', 99 }) == 300);
    assert(parking.GetCurrentBill({ 'C', 'C', 333, 'C', 99 }) == 0);
    parking.Withdraw({ 'B', 'B', 222, 'B', 99 });

    TestClock::SetNow(70);
    {
        // проверим счЄт
        auto bill = parking.EndPeriodAndGetBills();

        // так как внутри макроса используетс€ зап€та€,
        // нужно заключить его аргумент в дополнительные скобки
        assert((bill
            == unordered_map<VehiclePlate, int64_t, VehiclePlateHasher>{
                { {'A', 'A', 111, 'A', 99}, 400},
                { {'B', 'B', 222, 'B', 99}, 300 },
        }));
    }

    TestClock::SetNow(80);
    {
        // проверим счЄт
        auto bill = parking.EndPeriodAndGetBills();

        // так как внутри макроса используетс€ зап€та€,
        // нужно заключить его аргумент в дополнительные скобки
        assert((bill
            == unordered_map<VehiclePlate, int64_t, VehiclePlateHasher>{
                { {'A', 'A', 111, 'A', 99}, 100},
        }));
    }

    try {
        parking.Park({ 'A', 'A', 111, 'A', 99 });
        assert(false);
    }
    catch (ParkingException) {
    }

    try {
        parking.Withdraw({ 'B', 'B', 222, 'B', 99 });
        assert(false);
    }
    catch (ParkingException) {
    }

    cout << "Success!"s << endl;
}