
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L->Act = NULL;
	L->First = NULL;
	L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	//procházíme seznam od začátku do konce
	tDLElemPtr item = L->First;
	while (item != NULL)
	{
		//uvolnění prvku seznamu, uložení následujícího prvku k mazání
		tDLElemPtr next_ptr = item->rptr;
		free(item);
		item = next_ptr;
	}
	//stav po inicializaci
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr item = malloc(sizeof(struct tDLElem));

	if (item != NULL) //kontrola alokace nového prvku
	{
		//nový prvek - nemá předchůce, následníkem je současný první prvek
		item->data = val;
		item->lptr = NULL;
		item->rptr = L->First;

		//předchozím prvkem aktuálně prvního je nově vytvořený prvek
		if (L->First != NULL)
			L->First->lptr = item;
		L->First = item;

		//v případě prázdného seznamu je nový prvek zároveň posledním
		if (L->Last == NULL)
			L->Last = item;
	}
	else DLError();
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	tDLElemPtr item = malloc(sizeof(struct tDLElem));

	if (item != NULL)
	{
		//nový prvek - předchůdcem je aktuálně poslední prvek, následníka nemá
		item->data = val;
		item->lptr = L->Last;
		item->rptr = NULL;

		//následníkem aktuálně posledního prvku je nový prvek
		if (L->Last != NULL)
			L->Last->rptr = item;
		L->Last = item;

		//v případě prázdného seznamu je nový prvek zároveň prvním
		if (L->First == NULL)
			L->First = item;
	}
	else DLError();
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L->First != NULL)
	{
		*val = L->First->data;
	}
	else DLError();
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L->Last != NULL)
	{
		*val = L->Last->data;
	}
	else DLError();
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	if (L->First != NULL)
	{
		//ztráta aktivity, pokud byl první prvek aktivní
		if (L->First == L->Act)
			L->Act = NULL;

		//uvolnění paměti prvního prvku, nastavení jeho následníka jako nového prvního
		tDLElemPtr new_first = L->First->rptr;
		free(L->First);
		L->First = new_first;
	}
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L.
** Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/ 
	if (L->Last != NULL)
	{
		//ztráta aktivity, pokud byl poslední prvek aktivní
		if (L->Last == L->Act)
			L->Act = NULL;

		//uvolnění paměti posledního prvku, nastavení jeho předchůdce jako nového posledního
		tDLElemPtr new_last = L->Last;
		L->Last = L->Last->lptr;
		L->Last->rptr = NULL;
		free(new_last);
	}
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	if (L->Act != NULL && L->Act != L->Last)
	{
		//uložení mazaného prvku, nastavení jeho následníka jako následníka aktivního prvku
		tDLElemPtr item = L->Act->rptr;
		L->Act->rptr = item->rptr;

		//mazaný prvek je poslední, aktivní prvek je nový poslední
		if (item == L->Last)
			L->Last = L->Act;
		//jinak nastavíme předchůdce následníka mazaného prvku na aktivní prvek
		else
			item->rptr->lptr = L->Act;
		
		free(item);
	}
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	if (L->Act != NULL && L->Act != L->First)
	{
		//uložení mazaného prvku, nastavení jeho předchůdce jako předchůdce aktivního prvku
		tDLElemPtr item = L->Act->lptr;
		L->Act->lptr = item->lptr;

		//mazaný prvek je první, aktivní prvek je tedy nový první
		if (item == L->First)
			L->First = L->Act;
		//jinak nastavíme následníka předchůdce mazaného prvku na aktivní prvek
		else
			item->lptr->rptr = L->Act;

		free(item);
	}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if (L->Act != NULL)
	{
		tDLElemPtr item = malloc(sizeof(struct tDLElem));
		if (item != NULL)
		{
			//nastavení nového prvku, je následníkem právě aktivního prvku
			item->data = val;
			item->lptr = L->Act;
			item->rptr = L->Act->rptr;

			//nový prvek je nyní rovněž předchůdcem následníka aktivního prvku
			L->Act->rptr->lptr = item;
			L->Act->rptr = item;
		}
		else DLError();
	}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if (L->Act != NULL)
	{
		tDLElemPtr item = malloc(sizeof(struct tDLElem));
		if (item != NULL)
		{
			//nastavení nového prvku, je předchůdcem právě aktivního prvku
			item->data = val;
			item->lptr = L->Act->lptr;
			item->rptr = L->Act;

			//nový prvek je nyní rovněž následníkem předchůdce aktivního prvku
			L->Act->lptr->rptr = item;
			L->Act->lptr = item;
		}
	}
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	if (L->Act != NULL)
	{
		*val = L->Act->data;
	}
	else DLError();
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if (L->Act != NULL)
	{
		L->Act->data = val;
	}
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	if (L->Act != NULL)
	{
		L->Act = L->Act->rptr;
	}
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if (L->Act != NULL)
	{
		L->Act = L->Act->lptr;
	}
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	return L->Act != NULL;
}

/* Konec c206.c*/
