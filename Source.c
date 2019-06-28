//Adatbáziskezelő
//Keszitette: Bányi Máté - H1B2DD 
//Utolsó módosítás: 2017.11.17

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "Header.h"

void fv_accfileread(FILE *acc, acclist *list) //acc beolvasasa fajlbol
{
	acclist *p = list;
	char name[meret], pass[meret]; 
	while(fscanf_s(acc,"%s %s", &name, sizeof(name), &pass, sizeof(pass)) > 0)
	{	
		acclist *a = (acclist*)calloc(1, sizeof(acclist));
		strcpy_s(a->a.accname, sizeof(a->a.accname), name);
		strcpy_s(a->a.pass, sizeof(a->a.pass), pass);
		a->next = p->next;
		a->prew = a->next->prew; // a->next == p->next
		p->next = a;
		a->next->prew = a;		
	}
}

void fv_stockfileread(FILE *stock, adatlist *list)//adatok beolvasasa fajlbol
{
	char name[meret], brand[meret], price[meret], db[meret], code[meret];
	adatlist *p = list; int i;
	while (p->next != NULL) // lista vegere
		p = p->next;
	while (true)
	{
		fgets(name, meret, stock);
		if (name[0] == '#' && name[1] == '&' && name[2] == '@')
			return;
		fgets(price, meret, stock);
		fgets(db, meret, stock);
		fgets(code, meret, stock);
		fgets(brand, meret, stock);
		for (i = 0; i < meret && name[i] != '\0'; i++) // eltunteti \n-t
		{
			if (name[i] == '\n')
			{
				name[i] = '\0';
				break;
			}
		}
		for (i = 0; i < meret && brand[i] != '\0'; i++)
		{
			if (brand[i] == '\n')
			{
				brand[i] = '\0';
				break;
			}
		}
		adatlist *a = (adatlist*)calloc(1, sizeof(adatlist));
		strcpy_s(a->a.name, sizeof(a->a.name), name);
		strcpy_s(a->a.brand, sizeof(a->a.brand), brand);
		a->a.price = atoi(price);
		a->a.stock = atoi(db);
		a->a.code = atoi(code);
		a->next = p;
		a->prew = p->prew;
		p->prew = a;
		a->prew->next = a;
	}
}

void fv_adatlistprint(adatlist *list)//lista kiiratasa
{
	adatlist *p = list->next;
	if (p->next == NULL)
	{
		printf_s("A lista ures!\n");
		return;
	}
	printf_s("\nTermekek:\n");
	while (p->next != NULL)
	{
		printf_s("Nev: %s\nAr: %d Ft\nKeszleten: %d darab\nKod: %d\nGyarto: %s\n\n", p->a.name, p->a.price, p->a.stock, p->a.code, p->a.brand);
		p = p->next;
	}
}

void fv_stockprint(adatlist *list)//adatok kiirasa
{
	FILE *stock;
	fopen_s(&stock, "stock.txt", "w");
	adatlist *p = list->next;
	while (p->next != NULL)
	{
		fprintf_s(stock, "%s\n%d\n%d\n%d\n%s\n", p->a.name, p->a.price, p->a.stock, p->a.code, p->a.brand);
		p = p->next;
	}
	fprintf_s(stock,"#&@"); // vegjel
	fclose(stock);
}

void fv_free(acclist *head, adatlist *adathead)//felszabaditas
{
	fv_stockprint(adathead);
	free(head);
	free(adathead);
}

void fv_arrendez(adatlist *list, int a) // arredezo fv
{
	adatlist* p = list->next;
	adatlist *pseg;
	adatlist *ujelem;
	adatlist *kiirlist;
	int db = 0;
	if (p->next == NULL)
	{
		printf_s("A lista ures!");
		return;
	}
	while (p->next != NULL) // hany elemu a list
	{
		db++;
		p = p->next;
	}
	p = list->next;
	kiirlist = adatlistfree(); 
	while (p->next != NULL) // arrendezeshez hasznalt masolat
	{
		ujelem = (adatlist*)calloc(1, sizeof(adatlist));
		ujelem->a = p->a;
		ujelem->next = kiirlist->next;
		ujelem->prew = kiirlist;
		kiirlist->next = ujelem;
		ujelem->next->prew = ujelem;
		p = p->next;
	}
	ujelem = (adatlist*)calloc(1, sizeof(adatlist)); // atmeneti tarolo
	for (p = kiirlist->next; p->next != NULL; p=p->next)
	{	
		for (pseg = p; pseg->next != NULL; pseg = pseg->next)
		{
			if (pseg->a.price < p->a.price) //rendezes
			{
				ujelem->a = pseg->a;
				pseg->a = p->a;
				p->a = ujelem->a;
			}
		}
	}
	free(ujelem);
	if (a == 1) // kiiratas
	{
		for (pseg = kiirlist->next; pseg->next!= NULL; pseg = pseg->next)
		{
			printf_s("Nev: %s\nAr: %d Ft\nKeszleten: %d darab\nKod: %d\nGyarto: %s\n\n", pseg->a.name, pseg->a.price, pseg->a.stock, pseg->a.code, pseg->a.brand);
		}
	}
	pseg = kiirlist;
	while (pseg->next != NULL)
	{
		pseg = pseg->next;
	}
	if (a == 2)
	{	
		for (pseg->prew; pseg->prew!= NULL; pseg = pseg->prew)
		{
			printf_s("Nev: %s\nAr: %d Ft\nKeszleten: %d darab\nKod: %d\nGyarto: %s\n\n", pseg->a.name, pseg->a.price, pseg->a.stock, pseg->a.code, pseg->a.brand);
		}
	}
	free(kiirlist);
}

void fv_nevrendez(adatlist *list, int a) // nevrendezo fv - ugyan az mint az arrendezo, csak a ar osszehasonlitas helyett nem osszehasonlitas van
{
	adatlist *p = list->next;
	adatlist *pseg;
	adatlist *ujelem;
	adatlist *kiirlist;
	int db = 0;
	if (p->next == NULL)
	{
		printf_s("A lista ures!");
		return;
	}
	while (p->next != NULL)
	{
		db++;
		p = p->next;
	}
	p = list->next;
	kiirlist = adatlistfree();
	while (p->next != NULL)
	{
		ujelem = (adatlist*)calloc(1, sizeof(adatlist));
		ujelem->a = p->a;
		ujelem->next = kiirlist->next;
		ujelem->prew = kiirlist;
		kiirlist->next = ujelem;
		ujelem->next->prew = ujelem;
		p = p->next;
	}
	ujelem = (adatlist*)calloc(1, sizeof(adatlist));
	for (p = kiirlist->next; p->next != NULL; p = p->next)
	{
		for (pseg = p; pseg->next != NULL; pseg = pseg->next)
		{
			if (strcmp(pseg->a.name,p->a.name) == 1)
			{
				ujelem->a = pseg->a;
				pseg->a = p->a;
				p->a = ujelem->a;
			}
		}
	}
	free(ujelem);
	if (a == 1)
	{
		for (pseg = kiirlist->next; pseg->next != NULL; pseg = pseg->next)
		{
			printf_s("Nev: %s\nAr: %d Ft\nKeszleten: %d darab\nKod: %d\nGyarto: %s\n\n", pseg->a.name, pseg->a.price, pseg->a.stock, pseg->a.code, pseg->a.brand);
		}
	}
	pseg = kiirlist;
	while (pseg->next != NULL)
	{
		pseg = pseg->next;
	}
	if (a == 2)
	{
		for (pseg->prew; pseg->prew != NULL; pseg = pseg->prew)
		{
			printf_s("Nev: %s\nAr: %d Ft\nKeszleten: %d darab\nKod: %d\nGyarto: %s\n\n", pseg->a.name, pseg->a.price, pseg->a.stock, pseg->a.code, pseg->a.brand);
		}
	}
	free(kiirlist);
}

void fv_gyartolista(adatlist *list) // gyartolistazas
{
	char brand[meret]; int i;
	adatlist *p = list->next;
	if (p->next == NULL)
	{
		printf_s("A lista ures!\n");
		return;
	}
	printf_s("\nGyarto: "); //keresendo gyarto
	fgets(brand, meret, stdin);
	for (i = 0; i < meret && brand[i] != '\0'; i++) //\n kiszedese
	{
		if (brand[i] == '\n')
		{
			brand[i] = '\0';
			break;
		}
	}
	while (p->next != NULL) // ezen gyarto termekeinek kiiratasa a listan vegigmenve
	{
		if (strcmp(brand, p->a.brand) == 0)
		{
			printf_s("Nev: %s\nAr: %d Ft\nKeszleten: %d darab\nKod: %d\nGyarto: %s\n\n", p->a.name, p->a.price, p->a.stock, p->a.code, p->a.brand);
		}
		p = p->next;
	}
}

void fv_arkozott(adatlist *list) // arkozott fv
{
	char c, min[meret], max[meret]; int imin = 0, imax=0, lmax, lmin=0;
	adatlist *p = list->next;
	if (p->next == NULL)
	{
		printf_s("A lista ures!\n");
		return;
	}
	while (p->next != NULL) // max ar meghatarozasa 
	{
		if (p->a.price > imax)
		{
			imax = p->a.price;
		}
		p = p->next;
	}
	lmax = imax; // max ar mentese
	while (true)
	{
		printf_s("Min ar(1)\nMax ar(2)\nKereses(3)\nDefault(4)\nVissza(0)\n");
		scanf_s("%c", &c, sizeof(c));
		fv_fflush();
		if (c == '0')
			return;
		if (c == '1')
		{
			printf_s("Add meg a min arat: ");
			scanf_s("%s", &min, sizeof(min));
			fv_fflush();
			imin = atoi(min);
		}
		if (c == '2')
		{
			printf_s("Add meg a max arat: ");
			scanf_s("%s", &max, sizeof(max));
			fv_fflush();
			imax = atoi(max);
		}
		if (c == '3') // hatarok kozti elemek kiiratasa
		{
			p = list->next;
			while (p->next != NULL)
			{
				if(p->a.price <= imax && p->a.price >= imin)
					printf_s("Nev: %s\nAr: %d Ft\nKeszleten: %d darab\nKod: %d\nGyarto: %s\n\n", p->a.name, p->a.price, p->a.stock, p->a.code, p->a.brand);
				p = p->next;
			}
		}
		if (c == '4') // alapertelmezett hatarok
		{
			imin = lmin;
			imax = lmax;
		}
	}
}

void fv_raktaron(adatlist *list) //raktarton fv
{
	adatlist *p = list->next;
	if (p->next == NULL)
	{
		printf_s("A lista ures!\n");
		return;
	}
	while (p->next != NULL) // ahol a darabszam >0 kiiratasa
	{
		if(p->a.stock > 0)
			printf_s("Nev: %s\nAr: %d Ft\nKeszleten: %d darab\nKod: %d\nGyarto: %s\n\n", p->a.name, p->a.price, p->a.stock, p->a.code, p->a.brand);
		p = p->next;
	}
}

void fv_listaz(adatlist *list)
{
	char c;
	while (true) // valaszthato lehetosegek
	{
		printf_s("Listazas:\nAlapertelmezett listazas(1)\nAr szerint novekvo(2)\nAr szerint csokkeno(3)\nKereses egy gyarto termekeire(4)\nAr koze eso termekek keresese(5)\nRaktaron levo termekek(6)\nNev szerint novekvo(7)\nNev szerint csokkeno(8)\nVissza(0)\n");
		scanf_s("%c", &c, sizeof(c));
		fv_fflush();
		if (c == '0')
			return;
		if (c == '1')
			fv_adatlistprint(list);
		if (c == '2')
			fv_arrendez(list,1);
		if (c == '3')
			fv_arrendez(list,2); 
		if (c == '4')
			fv_gyartolista(list);
		if (c == '5')
			fv_arkozott(list);
		if (c == '6')
			fv_raktaron(list);
		if (c == '7')
			fv_nevrendez(list, 2);
		if (c == '8')
			fv_nevrendez(list, 1);
	}
}

void fv_vendeg(acclist *head, adatlist *adathead)//vendeg menu
{
	char c ;
	adatlist *p = adathead;
	while (true) //vendegek altal valaszthato lehetosegek
	{
		printf_s("Termeklista megtekintese - 1\nKijelentkezes - 2\nKilepes - 0\n");
		scanf_s("%c", &c, sizeof(c));
		fv_fflush();
		if (c == '1')
		{
			fv_listaz(adathead);
		}
		if (c == '2')
			return;
		if (c == '0')
		{
			fv_free(head, adathead);
			exit(0);
		}
		else
			continue;
	}
}

void fv_modify(adatlist *list)//dolgozo - modositas almenu
{
	char c[meret], d; int b,i;
	adatlist *p = list->next;
	printf_s("Modositani kivant elem kodja: "); scanf_s("%s", &c, sizeof(c)); b = atoi(c);
	fv_fflush();
	while (p->next != NULL)
	{
		if (p->a.code == b)
		{
			while (true) // modositando parameter valasztasa
			{
				printf_s("\nA kivalasztott elem adatai:\n");
				printf_s("Name(1): %s\nPrice(2): %d Ft\nStock(3): %d darab\nCode(4): %d\nBrand(5): %s\n\nVissza(0)\n", p->a.name, p->a.price, p->a.stock, p->a.code, p->a.brand);
				printf_s("A modositani kivant parameter: ");
				scanf_s("%c", &d, sizeof(d));
				fv_fflush();
				if (d == '0')
					return;
				if (d == '1') // uj nev
				{
					printf_s("Add meg az uj nevet: ");
					//fv_fflush();
					fgets(p->a.name, meret, stdin);
					for (i = 0; i < meret && p->a.name[i] != '\0'; i++) // eltunteti \n-t
					{
						if (p->a.name[i] == '\n')
						{
							p->a.name[i] = '\0';
							break;
						}
					}
				}
				if (d == '5') // uj gyarto
				{
					printf_s("Add meg az uj gyartot: ");
					//fv_fflush();
					fgets(p->a.brand, meret, stdin);
					for (i = 0; i < meret && p->a.brand[i] != '\0'; i++) // eltunteti \n-t
					{
						if (p->a.brand[i] == '\n')
						{
							p->a.brand[i] = '\0';
							break;
						}
					}
				}
				if (d == '2') // uj ar
				{
					printf_s("Uj ar: ");
					scanf_s("%s", &c, sizeof(c)); 
					p->a.price = abs(atoi(c));
				}
				if (d == '3') // uj db
				{
					printf_s("Uj darabszam: ");
					scanf_s("%s", &c, sizeof(c));
					p->a.stock = abs(atoi(c));
				}
				if (d == '4') // uj kod
				{
					printf_s("Uj kod: ");
					scanf_s("%s", &c, sizeof(c));
					p->a.code = abs(atoi(c));
				}
			}
		}
		p = p->next;
	}
	printf_s("\nIlyen koddal elem nem talalhato!\n");
}

void fv_newstock(adatlist *list)//uj adat hozzadasa a listahoz
{
	char c[meret], r; adatlist *p = list->next; int i;
	adatlist *t = list->next;
	adatlist *a = (adatlist*)calloc(1, sizeof(adatlist));
	fv_fflush();
	printf_s("Termek neve: "); fgets(a->a.name, meret, stdin);
	printf_s("Termek gyartoja: "); fgets(a->a.brand, meret, stdin);
	printf_s("Ar: "); scanf_s("%s", &c, sizeof(c)); a->a.price = abs(atoi(c));
	printf_s("Darabszam: "); scanf_s("%s", &c, sizeof(c)); a->a.stock = abs(atoi(c));
	printf_s("Kod: "); scanf_s("%s", &c, sizeof(c)); a->a.code = abs(atoi(c));
	while (t->next != NULL)
	{
		if (t->a.code == a->a.code)
		{
			printf_s("Ez a kod mar letezik!\n");
			printf_s("Kod: "); scanf_s("%s", &c, sizeof(c)); a->a.code = atoi(c);
			t = list->next;
			continue;
		}
		t = t->next;
	}
	for (i = 0; i < meret && a->a.name[i] != '\0'; i++) // namebol eltunteti \n-t
	{
		if (a->a.name[i] == '\n')
		{
			a->a.name[i] = '\0';
			break;
		}
	}
	for (i = 0; i < meret && a->a.brand[i] != '\0'; i++)
	{
		if (a->a.brand[i] == '\n')
		{
			a->a.brand[i] = '\0';
			break;
		}
	}
	printf_s("\nAkarod menteni az elemet? (i/n)\n\nA megadott adatok:\n");
	printf_s("Nev: %s\nAr: %d Ft\nKeszleten: %d darab\nKod: %d\nGyarto: %s\n\n", a->a.name, a->a.price, a->a.stock, a->a.code, a->a.brand);
	fv_fflush();
	while (true)
	{
		scanf_s("%c", &r, sizeof(r));
		if (r == 'i' || r == 'I')
		{
			while (p->next != NULL)
				p = p->next;
			a->next = p;
			a->prew = p->prew;
			p->prew = a;
			a->prew->next = a;
			return;
		}
		if (r == 'n' || r == 'N')
		{
			free(a);
			return;
		}
		else
			continue;
	}
}

void fv_delete(adatlist *list)
{
	adatlist *p = list->next; char s[meret]; int code;
	printf_s("Add meg a torleno elem kodjat: "); scanf_s("%s", &s, sizeof(s)); code = atoi(s);
	if (p->next == NULL)
	{
		printf_s("A list a ures!\n");
		return;
	}
	while (p->next != NULL && p->a.code != code)
		p = p->next;
	if (code == p->a.code)
	{
		p->prew->next = p->next;
		p->next->prew = p->prew;
		free(p);
		printf_s("Az %s kodu elem torolve!",s);
		return;
	}
	else
	{
		printf_s("\nA %s kodu elem nem torolheto, mivel nem letezik!\n", s);
	}
}

void fv_options(adatlist *list)//dolgozo - modosito almenu
{
	char a;
	while (true)
	{
		printf_s("\nUj elem hozzaadasa - 1\nMeglevo elem modositsa - 2\nElem torlese - 3\nKeszlet uritese - 4\nVissza - 0\n");
		scanf_s("%c", &a, sizeof(a));
		fv_fflush();
		if (a == '0')
			return;
		if (a == '4')
		{
			free(list);
			list = adatlistfree();
		}
		if (a == '2')
		{
			fv_modify(list);
		}

		if (a == '1')
		{
			fv_newstock(list);
		}
		if (a == '3')
		{
			fv_delete(list);
		}
		else
		{
			continue;
		}
	}
}

void fv_dolgozo(acclist *head, adatlist *adathead)//dolgozo menu
{
	char name[meret], pass[meret], c;
	acclist *accp = head->next;
	//login
	printf_s("Username: "); scanf_s("%s", &name, sizeof(name));
	printf_s("Password: "); scanf_s("%s", &pass, sizeof(pass));
	fv_fflush();
	while (accp->next != NULL)
	{
		if (strcmp(name, accp->a.accname) == 0)
		{
			if (strcmp(pass, accp->a.pass) == 0)
			{
				while (true)
				{
					printf_s("\nTermeklista megjelenitese - 1\nTermeklista modositasa - 2\nKijelentkezes - 3\nKilepes - 0\n");
					scanf_s("%c", &c, sizeof(c));
					fv_fflush();
					if (c == '1')
					{
						fv_listaz(adathead);
					}
					if (c == '0')
					{
						fv_free(head, adathead);
						exit(0);
					}
					if (c == '3')
						return;
					if (c == '2')
						fv_options(adathead);
				}
			}
		}
		accp = accp->next;
	}
	printf_s("Hibas felhasznalonev vs jelszo\n");
}

int main() 
{
	char c; 
	acclist *head = acclistfree();
	adatlist *adathead = adatlistfree();
	adatlist *test = adathead;
	FILE *acc, *stock;
	fopen_s(&acc, "account.txt", "r");
	fopen_s(&stock, "stock.txt", "r+");
	if (acc == NULL || stock == NULL)
	{
		printf_s("A szukseges fajlok nem elerhetok, ezert a program leall! \n");
		return 0;
	}
	fv_accfileread(acc, head);
	fclose(acc);
	fv_stockfileread(stock, adathead);
	fclose(stock);
	while (true)
	{
		printf_s("Bejelentkezes dolgozokent - 1\nBejelentkezes vendegkent - 2\nKilepes - 0\n");
		scanf_s("%c", &c, sizeof(c));
		fv_fflush();
		if (c == '2')
			fv_vendeg( head, adathead);
		if (c == '1')
			fv_dolgozo(head, adathead);
		if (c == '0')
			break;
		else
			continue;
	}	
	fv_free(head, adathead);
	return 0;
}