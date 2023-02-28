// Numan Yaman - 200202135     Bekir Güçlü - 200202132

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX 100

typedef struct Flight
{	
  int priority,plane,landing_request,landing_time,delay,takeoff_time;
  struct Flight *next,*back;
}Flight;

typedef struct Queue
{
  struct Flight *first,*last;
}Queue;

Queue *createQueue()
{			
	Queue *queue = (Queue *) malloc(sizeof(Queue));	// kuyruga ram de yer aç	
	queue->first = NULL; // ilk ve son eleman null deger ata dondur		
	queue->last = NULL;
	return queue;
}

Flight *copyFlight(Flight* copy) // kopyala yeni dugume ata
{		
	Flight *newFlight = (Flight *) malloc(sizeof(Flight));
	
	newFlight->delay = copy->delay;
	newFlight->plane = copy->plane;
	newFlight->landing_request = copy->landing_request;
	newFlight->priority = copy->priority;
	newFlight->landing_time = copy->landing_time;
	newFlight->takeoff_time = copy->takeoff_time;
	
	return newFlight;
}

Flight *firstDelete(Queue *queue) //ilk elemeaný kopyala döndür ve sil
{			
	Flight *employee = (Flight *) malloc(sizeof(Flight));
	employee = queue->first;
	
	if(queue->first != queue->last)
	{		
		queue->first = queue->first->next;
	}
	return employee;
}

void readInput(Queue *queue) //input.txt okuma 
{		
	int count;
	char row[MAX];
	bool isEnable = false;
	
    FILE *file;
	fopen_s(&file, "input.txt", "r");		
	Flight * temp = queue -> first;	//dosyayi kuyrugun ilk elemanýndan okumaya basla	
	
	while(fgets(row, 200, file)) // 1 satirda 200 okuma
	{			
		if(!isEnable) // ilk satirda input da string oldugu icin okumamasi
		{						
			isEnable = true;
		}
		else
		{				
			char * token = strtok(row, " "); 	//stringi yoket
			count = 0;
			Flight *newFlight = (Flight *) malloc(sizeof(Flight));	//yeni dugum	
			while (token != NULL)
		    {
		    	if(count == 0) //count 0 iken ilk cekilen priority
				{			
		    		newFlight -> priority = atoi(token);	
				}
				else if(count == 1) // 2.ucak
				{		
					newFlight -> plane = atoi(token);
				}
				else if(count == 2) //3.eleman inis talebi saati
				{
					newFlight -> landing_request = atoi(token);
					newFlight->delay= 0;
				}
				count++;		
		        token = strtok(NULL, " ");   
		    }
		    addElement(queue,newFlight); // olusturulan dugum kuyruga eklenir
		}  
	}
}

void createOutput(Queue *landing, Queue *descent) //output.txt oluþturuldu
{
	printf("\n***** Output.txt is Being Created *****\n");
	Flight *temp = landing->first;	//gecici bir eleman oluþtur ve iniþ listesi baþýna al.
		
	while(temp != NULL)
	{			
		temp->landing_time = temp->delay + temp->landing_request; // talep + gecikme
		
		if(temp->landing_time <= 24) // 24 saatlik dilim geçiyorsa baþka havaalanýna yönlendirme
		{							
			Flight *flag = copyFlight(temp);
			printFlight(flag);
			addElement(descent, flag);
		}
		else
		{
			printf("ID %d -> Plane Diverted To Another Airport.\n", temp->plane);
		}
		temp = temp->next;	// temp bir sonraki dugume gonderme.	
	}
	printOutput(descent);			
}

void printOutput(Queue *descent) // output.txt yazdirma
{			
	 FILE *file = fopen("output.txt", "w");
	 Flight *temp = descent->first;
    
	 fprintf(file, "Priority_ID\t Plane_ID\t Landing_Request \tLanding_Time \tDelay \tTakeoff_Time\n");
     fprintf(file,"----------------------------------------------------------------------------------------------------\n");
	 	 
	 while(temp != NULL)
	 {
	 	 fprintf(file, "    %d\t\t    ", temp->priority); 
	 	 fprintf(file, "%d\t\t\t", temp->plane); 
	 	 fprintf(file, "%d\t\t\t", temp->landing_request); 
	 	 fprintf(file, "%d\t   ", temp->landing_time); 
	 	 fprintf(file, "%d\t   ", temp->delay); 
	 	 fprintf(file, "%d\t\n",(temp->landing_time + 1) % 24);
		 fprintf(file, "----------------------------------------------------------------------------------------------------\n"); 
	 	 temp = temp->next ;
	 } 
	 fclose(file);
}

void addElement(Queue *queue,Flight *newFlight) // oncelige bakmaz sonra ekler
{		
	if(queue->first == NULL) //kuyruk bossa ile ve son eleman
	{						
		queue->first = newFlight;
		queue->last = newFlight;
		newFlight->next = NULL;
		newFlight->back = NULL;
	}
	else // bos degilse sonra ekler sonran sonra gelen yeni gelen yeni gelenden onceki son eleman kuyrugun sonu olur kendinden sonraki null alýr
	{				
		queue->last->next = newFlight;			
		newFlight->back = queue->last;			
		queue->last =newFlight;					
		queue->last->next=NULL;				
	}	
}

void requestedLanding(Queue *queue, Flight *newFlight) //inis saati ayarlama
{		
	if(queue->first == NULL) //ilk talep eden kuyrugun ilki ve sonu olur.
	{		
		newFlight->back = NULL;	
		newFlight->next = NULL;
		queue->first = newFlight;
		queue->last = newFlight;
		printf("ID %d -> Landing Request for Plane Time %d has been approved.\n",newFlight->plane, newFlight->landing_request);
		return;
	}
	else // talep edilen saat boþ mu dolu mu
	{						
		Flight *temp = queue->first;		
		bool isThere = false;
		
		while(temp!=NULL) //kuyrugu gez dolu mu boþ mu bak
		{
			if(temp->landing_request + temp->delay == newFlight->landing_request + newFlight->delay)
			{
				isThere = true;	 // talep edilen doluysa true		
			}
			temp = temp->next;		
		}
				
		if(isThere == false) // bos ise baþ son orta 3 olasýlýk
		{		
			temp = queue->first;	
			if(newFlight->landing_request + newFlight->delay <= temp->landing_request + temp->delay) //ilk sýraya ekleme
			{	 
				newFlight->back = NULL;	 // gelenin öncesi ilk olur	
				newFlight->next = temp;	
				temp->back = newFlight;	
				queue->first = newFlight; 		
				printf("ID %d -> Landing Request for Plane Time %d has been approved.\n",newFlight->plane, newFlight->landing_request + newFlight->delay);
				return;
			}
			
			if(newFlight->landing_request + newFlight->delay  > queue->last->landing_request + queue->last->delay) // son sýraya ekleme
			{ 
				newFlight->next = NULL;	//sonrasý boþ sonuncu olur
				queue->last->next = newFlight;		
				newFlight->back = queue->last;
				queue->last = newFlight;	
				printf("ID %d -> Landing Request for Plane Time %d has been approved.\n",newFlight->plane, newFlight->landing_request + newFlight->delay);
				return;
			}
			while(temp!=NULL)// orta 
			{                                                   
				if(newFlight->landing_request + newFlight->delay >= temp->landing_request + temp->delay)//inis saatinin sýrasýna göre
				{	
					temp=temp->next;
				}
				else //sonda yerini alýr
				{		
					temp->back->next = newFlight;
					newFlight->back = temp->back;
					temp->back = newFlight;
					newFlight->next = temp;
					break;
				}
			}
			printf("ID %d -> Landing Request for Plane Time %d has been approved.\n",newFlight->plane, newFlight->landing_request + newFlight->delay);	
		}
		else
		{			
			temp = queue->first; // yer doluysa önceliði varsa geçer yoksa baþka yere 
			while(true)
			{
				if(temp->landing_request + temp->delay == newFlight->landing_request + newFlight->delay )
				{
					break;
				}
				temp = temp->next;
			}		
			if(newFlight->priority < temp->priority)
			{		
				if(temp->delay != 3 && newFlight->delay == 3) //  yeni gelen delay max ise yerine gececegi max olmadýysa onun yerine geç
				{		
					printf("ID %d -> The plane took the place of the with ID->%d at %d time before the maximum delay.\n",newFlight->plane, newFlight->landing_request + newFlight->delay, temp->plane);
					changeFlight(newFlight, temp);		
				}
				else if(temp->delay == 3 && newFlight->delay != 3)// yeni gelen max delay yoksa gececegi max delay ise bekle
				{		
					newFlight->delay += 1;
					printf("ID %d-> The plane is delayed for 1 more hour.Because it couldn't find to seat.\n",newFlight->plane);
					requestedLanding(queue,newFlight);
					return;
				}
				else if(temp->delay == 3 && newFlight->delay == 3) //her ikisi max ise son gelen baþka yere
				{		
					printf("ID %d -> The plane is diverted to another airport without delaying the maximum.\n",newFlight->plane);
					return;
				}
				else // öncelik
				{			
					printf("ID %d -> The plane takes the place of the plane with ID -> %d from the landing priority.\n",newFlight->plane, temp->plane);
					changeFlight(newFlight, temp);
				}
				newFlight->delay += 1;		
				requestedLanding(queue, newFlight);		
				return;	
			}
			else // oncelik yetmiyorsa 
			{			
			    if(newFlight->delay == 3 && temp->delay != 3)	//max delay yerine geç
				{
			    	changeFlight(newFlight, temp);
			        printf("ID %d -> The plane takes the place of the plane with ID -> %d from the landing priority.\n",temp->plane,newFlight->plane);
			    	
				}
				else if(newFlight->delay == 3) // ikiside max delay ise baþka yere yönlendir.
				{		
					printf("ID %d -> The plane is diverted to another airport without delaying the maximum.\n",newFlight->plane);
					return;
				}
				newFlight->delay+=1;			
				requestedLanding(queue, newFlight);
				return;	
			}
		}	
	}	
}

void printQueue(Queue *queue) // icine aldýgý dugumu yazdir
{
	Flight *temp = queue->first;
	printf("\n");
	
	while(temp != NULL)
	{
		temp=temp->next;
	}
}

void printFlight(Flight *flight)
{   
    printf("-----------------------------------------------------------");
	printf("\n%d\t",flight->priority);
	printf("%d\t",flight->plane);
	printf("%d\t",flight->landing_request);
	printf("%d\t",flight->delay);
	printf("%d\t",flight->landing_time);
	printf("%d\n\n",(flight->takeoff_time) % 24);
}

void changeFlight(Flight *flight1, Flight *flight2) // iki ucagýn özellikleri takas eder
{			
	int priority1,priority2,plane1,plane2,delay1,delay2,takeoff_time1,takeoff_time2,landing_request1,landing_request2,landing_time1,landing_time2;
	
	priority1 = flight1->priority; 
	plane1 = flight1->plane; 
	delay1 = flight1->delay;
	takeoff_time1 = flight1->takeoff_time;
	landing_request1 = flight1->landing_request;
	landing_time1 = flight1->landing_time;

 	priority2 = flight2->priority;
	plane2 = flight2->plane;
	delay2 = flight2->delay;
	takeoff_time2 = flight2->takeoff_time;
	landing_request2 = flight2->landing_request;
	landing_time2 = flight1->landing_time;
	
	flight1->priority = priority2;
	flight1->plane = plane2;
	flight1->delay = delay2;
	flight1->takeoff_time = takeoff_time2;
	flight1->landing_request = landing_request2;
	flight1->landing_time = landing_time2;
	
	flight2->priority = priority1;
	flight2->plane = plane1;
	flight2->delay = delay1;
	flight2->takeoff_time = takeoff_time1;
	flight2->landing_request = landing_request1;
	flight2->landing_time = landing_time1;
}

int empty(Queue *queue) // icine aldýgý kuyruk ilk elemani bos ise true degilse false 
{ 
 	if(queue->first == NULL)
	{
		return 1;
	}
	return 0;
}

int main()
{
	Queue *data = createQueue();	//kuyruk olustur dondur	
    readInput(data);	//input.txt veri al ve bagli listeye sýra sýra at					
    
    Queue *airstrip = createQueue();	// kabul edilenler tutulur	
    Queue *takeoffrunway = createQueue();	// inis yapanlarýn kalkis bilgileri	
    
    Flight *nextPlane =(Flight *) malloc(sizeof(Flight));	//okunan veriyi requestedlandinge gonder nextplande tut	   
    
    while(1)
	{
    	nextPlane = firstDelete(data);		// nextplane ilk eleman gelir veri listesinden sil	
    	
    	requestedLanding(airstrip, nextPlane);	// inis talebi edilince yer varsa siraya ekle		
    	if(nextPlane == data->last) // son ucaga kadar devam et donguyu bitir
		{    							
    		break;
		}
	}
	createOutput(airstrip, takeoffrunway);	//output.txt oluþtur.	
    return 0;    
}
