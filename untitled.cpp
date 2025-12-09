#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>

using namespace std;
int cnt = 0, i, j, flag2, Myid,   vis = -1;
int  field;
char tmps[20];
int tmpi;
struct MyAccount {
	int money, dateint, flag_print;
	char signal, accname[20], date[20], name[20], type[20], ps[20];
} MyACC[1000], T;
void initialDateint() {
	for (i = 1; i <= cnt; i++) {
		int size = 1, di = 0;
		for (j = 7; j >= 0; j--) {
			di += size * (MyACC[i].date[j] - 48);
			size *= 10;
		}
		MyACC[i].dateint = di;
	}
}
void Hand_Input();
void File_Input();
void input() {
	printf("手动输入账务请输1，文件读取账务请输2\n");
	scanf("%d", &flag2);
	if (flag2 == 1)
		Hand_Input();
	else if (flag2 == 2)
		File_Input();
	initialDateint();
}
void Hand_Input() { //输入账目函数
	int flag;
	printf("您需要录入多少条账务记录？\n");
	scanf("%d", &flag);
	getchar();
	while (flag--) {
		cnt++;
		Myid = cnt;
		printf("请按顺序输入以下内容:\n开户人 收支金额 交易日期 收支项名称 收支类型 备注信息\n");
		scanf("%s", MyACC[Myid].accname);
		scanf(" %c", &MyACC[Myid].signal);
		scanf("%d", &MyACC[Myid].money);
		scanf("%s", MyACC[Myid].date);
		scanf("%s", MyACC[Myid].name);
		scanf("%s", MyACC[Myid].type);
		scanf("%s", MyACC[Myid].ps);
		getchar();
		printf("第%d条录入完成！\n", cnt);
	}
}
void File_Input() {
	FILE *fp;
	if ((fp = fopen("in.txt", "r+")) == NULL) {
		printf("无法打开文件!请检查是否建立输入文件！\n");
		exit(0);
	} else {
		cnt = 1;
		while (fscanf(fp, "id:%d 开户人:%s  余额:%c%d  日期:%s  收支项名称:%s  收支项类型:%s  备注:%s",
		              &cnt, MyACC[cnt].accname, &MyACC[cnt].signal, &MyACC[cnt].money, MyACC[cnt].date, MyACC[cnt].name, MyACC[cnt].type, MyACC[cnt].ps) != EOF) {
			cnt++;
		}

		cnt--;
		printf("输入完成，共%d条账目", cnt);
	}
}
void load_from_file() {
	FILE *fp = fopen("out.txt", "r");
	if (!fp) {
		printf("未找到 out.txt，将以空账本启动。\n");
		return;
	}
	
	cnt = 0;
	int id;
	while (fscanf(fp, "%d %19s %c %d %19s %19s %19s %19s",
		&id,
		MyACC[cnt + 1].accname,
		&MyACC[cnt + 1].signal,
		&MyACC[cnt + 1].money,
		MyACC[cnt + 1].date,
		MyACC[cnt + 1].name,
		MyACC[cnt + 1].type,
		MyACC[cnt + 1].ps) == 8) {
		cnt++;
	}
	fclose(fp);
	initialDateint();
	printf("已从 out.txt 加载 %d 条账目记录。\n", cnt);
}
void del() { //删除账目函数
	printf("请输入需要删除的记录ID:\n");
	scanf("%d", &Myid);
	//memset(&MyACC[Myid],0,sizeof(MyACC[Myid]));
	MyACC[Myid].accname[0] = 0;
	MyACC[Myid].signal = '+';
	MyACC[Myid].money = 0;
	MyACC[Myid].date[0] = '0';
	MyACC[Myid].name[0] = 0;
	MyACC[Myid].type[0] = 0;
	MyACC[Myid].ps[0] = 0;
	printf("删除成功！\n\n");
}
void chaxun() {
	printf("查询全部记录输入1，按字段查询输入2\n");
	scanf("%d", &flag2);
	if (flag2 == 1) {  //查询全部记录
		printf("查询成功！\n\n");
		for (i = 1; i <= cnt; i++)
			printf("id:%d 开户人:%s  余额:%c%d  日期:%s  收支项名称:%s  收支项类型:%s  备注:%s\n\n", i, MyACC[i].accname, MyACC[i].signal,
			       MyACC[i].money, MyACC[i].date, MyACC[i].name, MyACC[i].type, MyACC[i].ps);
	} else {
		printf("请输入查询所依据的字段\n1：序号\n2：日期\n3：名称\n4：金额\n");
		scanf("%d", &flag2);
		printf("请输入想查询字段的内容\n");
		if (flag2 == 1) {
			scanf("%d", &tmpi);
			printf("查询成功！\n\n");
			printf("id:%d 开户人:%s  余额:%c%d  日期:%s  收支项名称:%s  收支项类型:%s  备注:%s\n\n", tmpi, MyACC[tmpi].accname, MyACC[tmpi].signal,
			       MyACC[tmpi].money, MyACC[tmpi].date, MyACC[tmpi].name, MyACC[tmpi].type, MyACC[tmpi].ps);
		} else if (flag2 == 2) {
			scanf("%s", tmps);
			for (i = 1; i <= cnt; i++) {
				if (strcmp(MyACC[i].date, tmps) == 0)
					printf("id:%d 开户人:%s  余额:%c%d  日期:%s  收支项名称:%s  收支项类型:%s  备注:%s\n\n", i, MyACC[i].accname, MyACC[i].signal,
					       MyACC[i].money, MyACC[i].date, MyACC[i].name, MyACC[i].type, MyACC[i].ps);
			}
		} else if (flag2 == 3) {
			scanf("%s", tmps);
			for (i = 1; i <= cnt; i++) {
				if (strcmp(MyACC[i].name, tmps) == 0)
					printf("id:%d 开户人:%s  余额:%c%d  日期:%s  收支项名称:%s  收支项类型:%s  备注:%s\n\n", i, MyACC[i].accname, MyACC[i].signal,
					       MyACC[i].money, MyACC[i].date, MyACC[i].name, MyACC[i].type, MyACC[i].ps);
			}
		} else if (flag2 == 4) {
			char tmp;
			cin >> tmp;
			cin >> tmpi;
			for (i = 1; i <= cnt; i++) {
				if (tmp == MyACC[i].signal && tmpi == MyACC[i].money)
					printf("id:%d 开户人:%s  余额:%c%d  日期:%s  收支项名称:%s  收支项类型:%s  备注:%s\n\n", i, MyACC[i].accname, MyACC[i].signal,
					       MyACC[i].money, MyACC[i].date, MyACC[i].name, MyACC[i].type, MyACC[i].ps);
			}
		}
		printf("查询完毕！\n");
	}
}
void change() {
	printf("请输入需要修改的账目序号、账目信息类型、修改后的内容,格式如：\n25 2 20191111\n\n注意：账目信息类型中1代表收支金额，2代表开户人姓名，3代表交易日期，4代表收支项名称，5代表收支类型，6代表备注信息\n");
	//记得加上&
	scanf("%d", &Myid);
	scanf("%d", &field);
	//getchar()吞掉换行符
	getchar();
	if (field == 1)
		scanf("%c%d", &MyACC[Myid].signal, &MyACC[Myid].money);
	else if (field == 2)
		scanf("%s", MyACC[Myid].accname);
	else if (field == 3)
		scanf("%s", MyACC[Myid].date);
	else if (field == 4)
		scanf("%s", MyACC[Myid].name);
	else if (field == 5)
		scanf("%s", MyACC[Myid].type);
	else if (field == 6)
		scanf("%s", MyACC[Myid].ps);
	printf("id:%d 开户人:%s  余额:%c%d  日期:%s  收支项名称:%s  收支项类型:%s  备注:%s\n\n",
		Myid, MyACC[Myid].accname, MyACC[Myid].signal,
		MyACC[Myid].money, MyACC[Myid].date, MyACC[Myid].name,
		MyACC[Myid].type, MyACC[Myid].ps);
	
}
void save() {
	FILE *fp;
	if ((fp = fopen("out.txt", "w")) == NULL) {
		printf("无法打开 out.txt 进行写入！\n");
		return;
	} else {
		for (i = 1; i <= cnt; i++) {
			fprintf(fp, "%d %s %c %d %s %s %s %s\n",
				i, MyACC[i].accname, MyACC[i].signal,
				MyACC[i].money, MyACC[i].date, MyACC[i].name,
				MyACC[i].type, MyACC[i].ps);
		}
		printf("保存成功！\n");
	}
	fclose(fp);
}
bool compare_money(MyAccount A, MyAccount B) {
	if (A.money > B.money)
		return true;
	else
		return false;
}
void moneysort() {
	//由于vis没有赋初值，所以当不存在负数时，会runtime error
	for (i = 1; i <= cnt - 1; i++) {
		for (j = 1; j <= cnt - 1; j++) {
			//如果是-的放到后面去
			if (MyACC[j].signal == '-') {
				T = MyACC[j];
				MyACC[j] = MyACC[j + 1];
				MyACC[j + 1] = T;
			}
		}
	}
	for (i = 1; i <= cnt; i++)
		if (MyACC[i].signal == '-') {
			vis = i;
			break;
		}
	if (vis != -1) {
		//存在支出
		sort(MyACC + 1, MyACC + vis, compare_money);
		sort(MyACC + vis, MyACC + cnt + 1, compare_money);
	} else {
		sort(MyACC + 1, MyACC + cnt + 1, compare_money);
	}

	for (i = 1; i <= cnt; i++) {
		printf("id:%d 开户人:%s  余额:%c%d  日期:%s  收支项名称:%s  收支项类型:%s  备注:%s\n\n", i, MyACC[i].accname, MyACC[i].signal, MyACC[i].money, MyACC[i].date, MyACC[i].name, MyACC[i].type, MyACC[i].ps);
	}
	printf("已按金额递减排序完成!\n");
}
void data_report() {
	printf("请输入需要统计的日期区间，如：20190909 20190918\n");
	int a, b;
	scanf("%d%d", &a, &b);
	printf("\n%d%d", a, b);
	int sumplus = 0, summinus = 0, sum = 0;
	for (j = 1; j <= cnt; j++) {
		if (MyACC[j].dateint >= a && MyACC[j].dateint <= b) {
			if (MyACC[j].signal == '+') {
				sum += MyACC[j].money;
				sumplus += MyACC[j].money;
			} else {
				sum -= MyACC[j].money;
				summinus += MyACC[j].money;
			}
		}
	}
	printf("账务统计完成！\n在此阶段，收入：%d元，支出：%d元，结余：%d元\n\n", sumplus, summinus, sum);
}
bool compare_date(MyAccount A, MyAccount B) {
	if (A.dateint < B.dateint)
		return true;
	else
		return false;
}
void datasort() {
	sort(MyACC + 1, MyACC + 1 + cnt, compare_date);
	for (i = 1; i <= cnt; i++) {
		printf("id:%d 开户人:%s  余额:%c%d  日期:%s  收支项名称:%s  收支项类型:%s  备注:%s\n\n", i, MyACC[i].accname, MyACC[i].signal, MyACC[i].money, MyACC[i].date, MyACC[i].name, MyACC[i].type, MyACC[i].ps);
	}
	printf("根据日期排序完成！\n");
}
void name_sort() {
	for (int i = 1; i <= cnt; i++)
		MyACC[i].flag_print = 0;		//0表示没有被输出过
	for (int i = 1; i <= cnt; i++) {
		if (!MyACC[i].flag_print) {
			printf("id:%d 开户人:%s  余额:%c%d  日期:%s  收支项名称:%s  收支项类型:%s  备注:%s\n\n", i, MyACC[i].accname, MyACC[i].signal, MyACC[i].money, MyACC[i].date, MyACC[i].name, MyACC[i].type, MyACC[i].ps);
			MyACC[i].flag_print = 1;
		}
		for (int j = i + 1; j <= cnt; j++) {
			if (!strcmp(MyACC[i].name, MyACC[j].name) && !MyACC[j].flag_print) {
				printf("id:%d 开户人:%s  余额:%c%d  日期:%s  收支项名称:%s  收支项类型:%s  备注:%s\n\n", i, MyACC[i].accname, MyACC[i].signal, MyACC[i].money, MyACC[i].date, MyACC[i].name, MyACC[i].type, MyACC[i].ps);
				MyACC[j].flag_print = 1;
			}
		}
	}
}
void acc_sort() {
	printf("根据金额排序输入1,根据日期排序输入2,根据名称排序输入3\n");
	scanf("%d", &flag2);
	if (flag2 == 1)
		moneysort();
	else if (flag2 == 2)
		datasort();
	else if (flag2 == 3)
		name_sort();
}
void UI() {
	cout << endl << endl;
	cout << "***************************************************************" << endl;
	cout << "**           请输入以下选项编号进行下一步操作!               **" << endl;
	cout << "**               1.输入账目                                  **" << endl;
	cout << "**               2.删除账目明细条目                          **" << endl;
	cout << "**               3.查询账目明细条目                          **" << endl;
	cout << "**               4.修改账目明细条目                          **" << endl;
	cout << "**               5.账务数据排序                              **" << endl;
	cout << "**               6.财务统计                                  **" << endl;
	cout << "**               7.保存文件                                  **" << endl;
	cout << "*****************0.退出系统************************************" << endl;
}

void clean_stdin() {
	cin.clear();
	cin.sync();
}

int main() {
	cout << "***********欢迎进入个人帐本管理系统****************************" << endl;
	system("date /T");
	system("TIME /T");
	load_from_file();
	UI();
	clean_stdin();

	while (true) {
		cout << "\n请输入操作编号：";
		clean_stdin();
		if (scanf("%d", &flag2) != 1) continue;

		switch (flag2) {
			case 1:
				input();
				break;
			case 2:
				del();
				break;
			case 3:
				chaxun();
				break;
			case 4:
				change();
				break;
			case 5:
				acc_sort();
				break;
			case 6:
				data_report();
				break;
			case 7:
				save();
				break;
			case 0:
				exit(0);
			default:
				printf("无效输入！\n");
				break;
		}

		UI();
	}
	return 0;
}


