#include <bits/stdc++.h>
using namespace std;

// -------------------- 日期类 --------------------
struct Date {
	int day; // 简化：一年中的第几天 (1~365)
	Date(int d = 1) : day(d) {}
	int operator-(const Date& other) const { return day - other.day; }
};

// -------------------- 抽象账户基类 --------------------
class Account {
protected:
	string id;          // 账号
	string name;        // 户名
	double balance;     // 余额
	double rate;        // 年利率，例如 0.035
	Date lastDate;      // 上次业务日期
	double accumulation;// 余额×天数 的累计值
	static int totalCount; // 静态成员：账户总数
	
public:
	Account(const string& id_, const string& name_,
		double rate_, const Date& d, double balance_ = 0.0)
	: id(id_), name(name_), balance(balance_),
	rate(rate_), lastDate(d), accumulation(0.0) {
		++totalCount;
	}
	
	virtual ~Account() {}
	
	const string& getId() const { return id; }
	const string& getName() const { return name; }
	double getBalance() const { return balance; }
	double getRate() const { return rate; }
	
	// 利息基数累计：把 [lastDate, d) 之间的 balance*天数 加到 accumulation
	void accumulate(const Date& d) {
		int days = d - lastDate;
		if (days > 0) {
			accumulation += balance * days;
			lastDate = d;
		}
	}
	
	// 存款（所有账户通用）
	virtual bool deposit(const Date& d, double amount) {
		if (amount <= 0) return false;
		accumulate(d);
		balance += amount;
		cout << "[存款成功] 账户 " << id << " 当前余额: " << balance << endl;
		return true;
	}
	
	// 取款（纯虚函数，由派生类按规则实现）
	virtual bool withdraw(const Date& d, double amount) = 0;
	
	// 结息（纯虚函数）
	virtual void settle(const Date& d) = 0;
	
	// 显示基础信息
	virtual void show() const {
		cout << "ID: " << id
		<< "  Name: " << name
		<< "  Balance: " << balance
		<< "  Rate: " << rate;
	}
	
	// 返回账户类型字符串（如 SAVING / CREDIT）
	virtual string getType() const = 0;
	
	// 友元输出运算符重载（配合多态）
	friend ostream& operator<<(ostream& os, const Account& acc) {
		acc.show();
		return os;
	}
	
	// 序列化到一行文本：type id name balance rate lastDate accumulation ...
	virtual void save(ostream& os) const {
		os << getType() << ' '
		<< id << ' ' << name << ' '
		<< balance << ' ' << rate << ' '
		<< lastDate.day << ' ' << accumulation;
	}
	
	static int getTotalCount() { return totalCount; }
};

int Account::totalCount = 0;

// -------------------- 储蓄账户 --------------------
class SavingsAccount : public Account {
public:
	SavingsAccount(const string& id_, const string& name_,
		double rate_, const Date& d, double balance_ = 0.0)
	: Account(id_, name_, rate_, d, balance_) {}
	
	// 不允许透支
	bool withdraw(const Date& d, double amount) override {
		if (amount <= 0) return false;
		if (amount > balance) {
			cout << "[取款失败] 余额不足。" << endl;
			return false;
		}
		accumulate(d);
		balance -= amount;
		cout << "[取款成功] 账户 " << id << " 当前余额: " << balance << endl;
		return true;
	}
	
	// 年终结息
	void settle(const Date& d) override {
		accumulate(d);
		const int yearDays = 365;
		double interest = accumulation / yearDays * rate;
		if (interest != 0) {
			balance += interest;
			cout << "[结息] 储蓄账户 " << id
			<< " 本期利息: " << interest
			<< "  新余额: " << balance << endl;
		}
		accumulation = 0; // 清零，为下一年准备
	}
	
	string getType() const override { return "SAVING"; }
	
	void save(ostream& os) const override {
		Account::save(os); // 无额外字段
	}
	
	void show() const override {
		cout << "[储蓄账户] ";
		Account::show();
		cout << endl;
	}
};

// -------------------- 信用卡账户 --------------------
class CreditAccount : public Account {
	double creditLimit; // 信用额度
public:
	CreditAccount(const string& id_, const string& name_,
		double rate_, const Date& d,
		double limit_, double balance_ = 0.0)
	: Account(id_, name_, rate_, d, balance_), creditLimit(limit_) {}
	
	// 可透支到 -creditLimit
	bool withdraw(const Date& d, double amount) override {
		if (amount <= 0) return false;
		accumulate(d);
		if (balance - amount < -creditLimit) {
			cout << "[取款失败] 超出信用额度。" << endl;
			return false;
		}
		balance -= amount;
		cout << "[刷卡/取现成功] 信用账户 " << id
		<< " 当前余额(负为欠款): " << balance << endl;
		return true;
	}
	
	// 对欠款部分计息（只在整体为负时生效）
	void settle(const Date& d) override {
		accumulate(d);
		const int yearDays = 365;
		double interest = 0.0;
		if (accumulation < 0) {
			// accumulation 为负，interest 也为负（需要偿还的利息）
			interest = accumulation / yearDays * rate;
			balance += interest;
			cout << "[结息] 信用账户 " << id
			<< " 本期利息(为负表示需要偿还): " << interest
			<< "  新余额: " << balance << endl;
		}
		accumulation = 0;
	}
	
	void show() const override {
		cout << "[信用账户] ";
		Account::show();
		cout << "  CreditLimit: " << creditLimit << endl;
	}
	
	string getType() const override { return "CREDIT"; }
	
	void save(ostream& os) const override {
		Account::save(os);
		os << ' ' << creditLimit;
	}
};

// -------------------- 银行系统 --------------------
class BankSystem {
	vector<Account*> accounts; // 多态存储不同账户
	Date currentDate;          // 系统当前日
public:
	BankSystem() : currentDate(1) {}
	
	~BankSystem() {
		for (auto p : accounts) delete p;
	}
	
	// 按 ID 查找账户
	Account* findAccount(const string& id) {
		for (auto p : accounts)
			if (p->getId() == id) return p;
		return nullptr;
	}
	
	// 开户
	void openAccount() {
		int type;
		cout << "请选择账户类型 (1-储蓄账户  2-信用卡账户): ";
		cin >> type;
		
		string id, name;
		double rate, initBalance;
		cout << "输入账户ID: ";
		cin >> id;
		if (findAccount(id)) {
			cout << "该ID已存在。" << endl;
			return;
		}
		cout << "输入姓名: ";
		cin >> name;
		cout << "输入年利率 (如 0.035): ";
		cin >> rate;
		cout << "输入开户日 (1~365): ";
		int day;
		cin >> day;
		if (day < 1 || day > 365) {
			cout << "日期非法。" << endl;
			return;
		}
		currentDate.day = max(currentDate.day, day);
		cout << "输入初始存款 (可为0): ";
		cin >> initBalance;
		
		if (type == 1) {
			accounts.push_back(new SavingsAccount(id, name, rate, Date(day), initBalance));
		} else if (type == 2) {
			double limit;
			cout << "输入信用额度: ";
			cin >> limit;
			accounts.push_back(new CreditAccount(id, name, rate, Date(day), limit, initBalance));
		} else {
			cout << "未知账户类型。" << endl;
			return;
		}
		cout << "开户成功！当前总账户数: " << Account::getTotalCount() << endl;
	}
	
	// 存款
	void deposit() {
		string id;
		cout << "输入账户ID: ";
		cin >> id;
		Account* acc = findAccount(id);
		if (!acc) {
			cout << "账号不存在。" << endl;
			return;
		}
		int day;
		double amount;
		cout << "输入操作日期 (1~365，需 >= 当前日 " << currentDate.day << "): ";
		cin >> day;
		if (day < currentDate.day) {
			cout << "日期不能倒退。" << endl;
			return;
		}
		if (day < 1 || day > 365) {
			cout << "日期非法。" << endl;
			return;
		}
		currentDate.day = day;
		cout << "输入存款金额: ";
		cin >> amount;
		acc->deposit(currentDate, amount);
	}
	
	// 取款 / 刷卡
	void withdraw() {
		string id;
		cout << "输入账户ID: ";
		cin >> id;
		Account* acc = findAccount(id);
		if (!acc) {
			cout << "账号不存在。" << endl;
			return;
		}
		int day;
		double amount;
		cout << "输入操作日期 (1~365，需 >= 当前日 " << currentDate.day << "): ";
		cin >> day;
		if (day < currentDate.day) {
			cout << "日期不能倒退。" << endl;
			return;
		}
		if (day < 1 || day > 365) {
			cout << "日期非法。" << endl;
			return;
		}
		currentDate.day = day;
		cout << "输入取款金额: ";
		cin >> amount;
		acc->withdraw(currentDate, amount);
	}
	
	// 年终结息（对所有账户）
	void settle() {
		int day;
		cout << "输入结息日期 (通常为 365): ";
		cin >> day;
		if (day < currentDate.day) {
			cout << "日期不能倒退。" << endl;
			return;
		}
		if (day < 1 || day > 365) {
			cout << "日期非法。" << endl;
			return;
		}
		currentDate.day = day;
		for (auto acc : accounts) {
			acc->settle(currentDate);
		}
	}
	
	// 单个账户查询
	void showAccount() {
		string id;
		cout << "输入账户ID: ";
		cin >> id;
		Account* acc = findAccount(id);
		if (!acc) {
			cout << "账号不存在。" << endl;
			return;
		}
		acc->show();
	}
	
	// 列出全部账户
	void listAll() {
		cout << "======= 所有账户信息 =======" << endl;
		for (auto acc : accounts) {
			acc->show();
		}
	}
	
	// 保存到文件
	void saveToFile(const string& filename) {
		ofstream ofs(filename);
		if (!ofs) {
			cout << "保存文件失败。" << endl;
			return;
		}
		ofs << currentDate.day << '\n';
		ofs << accounts.size() << '\n';
		for (auto acc : accounts) {
			acc->save(ofs);
			ofs << '\n';
		}
		cout << "已保存到文件 " << filename << endl;
	}
	
	// 从文件载入
	void loadFromFile(const string& filename) {
		ifstream ifs(filename);
		if (!ifs) {
			cout << "未找到数据文件，使用空系统。" << endl;
			return;
		}
		for (auto p : accounts) delete p;
		accounts.clear();
		
		int day;
		size_t n;
		ifs >> day;
		currentDate.day = day;
		ifs >> n;
		for (size_t i = 0; i < n; ++i) {
			string type;
			ifs >> type;
			string id, name;
			double balance, rate;
			int lastDay;
			double accumu;
			ifs >> id >> name >> balance >> rate >> lastDay >> accumu;
			if (type == "SAVING") {
				auto* p = new SavingsAccount(id, name, rate, Date(lastDay), balance);
				// 为简单起见，忽略 accumu 的恢复，新的一年重新累计
				accounts.push_back(p);
			} else if (type == "CREDIT") {
				double limit;
				ifs >> limit;
				auto* p = new CreditAccount(id, name, rate, Date(lastDay), limit, balance);
				accounts.push_back(p);
			}
		}
		cout << "从文件 " << filename << " 载入 " << accounts.size() << " 个账户。" << endl;
	}
	
	// 主循环
	void run() {
		const string datafile = "accounts.txt";
		loadFromFile(datafile);
		
		int choice;
		do {
			cout << "\n====== 个人银行账户管理系统 ======\n";
			cout << "当前日: " << currentDate.day << endl;
			cout << "1. 开户\n";
			cout << "2. 存款\n";
			cout << "3. 取款/刷卡\n";
			cout << "4. 账户查询\n";
			cout << "5. 所有账户列表\n";
			cout << "6. 年终结息\n";
			cout << "0. 退出并保存\n";
			cout << "请选择: ";
			if (!(cin >> choice)) break;
			
			switch (choice) {
				case 1: openAccount(); break;
				case 2: deposit(); break;
				case 3: withdraw(); break;
				case 4: showAccount(); break;
				case 5: listAll(); break;
				case 6: settle(); break;
				case 0: saveToFile(datafile); break;
				default: cout << "无效选择。" << endl; break;
			}
		} while (choice != 0);
	}
};

// -------------------- main --------------------
int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	
	BankSystem system;
	system.run();
	return 0;
}

