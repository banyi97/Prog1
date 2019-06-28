#pragma once

#define meret 20

typedef struct account
{
	char accname[meret];
	char pass[meret];
}account;

typedef struct acclist
{
	account a;
	struct acclist *next;
	struct acclist *prew;
}acclist;

typedef struct adat {
	char name[meret];
	int stock;
	char brand[meret];
	int price;
	int code;

}adat;

typedef struct adatlist {
	adat a;
	struct adatlist *next;
	struct adatlist *prew;
}adatlist;

inline void fv_fflush() // stdio fflush 
{
	while (getchar() != '\n'); // puffer uritese
}

acclist *acclistfree() //acc strazsa
{
	acclist *a = (acclist*)calloc(1, sizeof(acclist));
	acclist *b = (acclist*)calloc(1, sizeof(acclist));
	a->next = b;
	b->prew = a;
	a->prew = NULL; b->next = NULL;
	return a;
}

adatlist *adatlistfree()//adat strazsa
{
	adatlist *a = (adatlist*)calloc(1, sizeof(adatlist));
	adatlist *b = (adatlist*)calloc(1, sizeof(adatlist));
	a->next = b;
	b->prew = a;
	a->prew = NULL; b->next = NULL;
	return a;
}
