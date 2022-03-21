/*
 * @Author: Distiny
 * @Date: 2022-03-19 20:33:18
 * @LastEditors: Distiny
 * @LastEditTime: 2022-03-21 15:54:22
 * @FilePath: \lab1\cpp_source\memory_manage.c
 * @Description:
 *
 * Copyright (c) 2022 by Distiny, All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define ALL_MEMORY 1000

struct map
{
   unsigned m_size;
   char *m_addr;
   struct map *next, *prior;
};

typedef struct map map;

char *BASE = NULL; // the base address

/**
 * @*description: Initial the whole program, set the available memory ALL_MEMORY
 * @param {int} *map_num map的总数
 * @param {map} *head  头指针
 * @return {*}
 */
void initial(int *map_num, map **head)
{
   printf("***********************************************\n");
   BASE = (char *)malloc(ALL_MEMORY);
   //内存申请失败返回 空指针
   if (BASE == NULL)
   {
      fprintf(stderr, "error:initail fail!\n");
      exit(-1);
   }
   else
   {
      printf("you obtain %d Byte memory and the head address is %d.\n", ALL_MEMORY, BASE);
   }
   *head = (map *)malloc(sizeof(map));
   if (*head == NULL)
   {
      fprintf(stderr, "error:initail fail!\n");
      exit(-1);
   }
   (*head)->m_addr = BASE;
   (*head)->m_size = 1000;
   (*head)->next = *head;
   (*head)->prior = *head;
   *map_num += 1;
   // printf("%d,%d,%d,%d",head->m_addr,head->m_size,head->next,head->prior);
}

/**
 * @*description: print the current map
 * @param {map} *m_map : the current map
 * @return {*}
 */
void printmap(map *m_map)
{
   printf("************************************************************\n");
   printf("The message about this map of link is shown as follows.\n");
   printf("The size is :               \t%d.\n", m_map->m_size);
   printf("The beginning address is:   \t%d.\n", m_map->m_addr);
   printf("The next map is:            \t%d.\n", (m_map->next)->m_addr);
   printf("The prior map is:           \t%d.\n", (m_map->prior)->m_addr);
   printf("************************************************************\n");
};

/**
 * @*description:
 * @param {map} *m_map the head pointer
 * @return {*}
 */
void print_all_map(map *m_map, int num)
{
   printf("************************************************************\n");
   printf("The quantity of maps is : %d\n", num);
   if (m_map == NULL)
   {
      printf("Error! There isn't map!");
      printf("************************************************************\n");
      return;
   }
   printf("num               address                size\n");
   map *temp = m_map;
   int i = 1;
   do
   {
      printf("%d                 %d                %d\n", i, temp->m_addr, temp->m_size);
      i += 1;
      temp = temp->next;
   } while (temp != m_map);
   printf("************************************************************\n");
}

/**
 * @*description: Applying for Memory Space
 * @param {unsigned} size
 * @param {int} *num
 * @param {map} *
 * @return {*}
 */
void lmalloc(unsigned size, int *num, map **head)
{
   if (size > 1000 || size <= 0)
   {
      printf("We can only handle memory 0-1000");
   }
   map *temp = *head;
   int flag = 0;
   do
   {
      if (temp->m_size > size)
      {
         flag = 1;
         (*head) = temp;
         (*head)->m_addr = ((*head)->m_addr + size);
         (*head)->m_size -= size;
         break;
      }
      else if (temp->m_size == size)
      {
         flag = 1;
         (*head) = temp->next;
         (*head)->prior = temp->prior;
         temp = temp->prior;
         temp->next = (*head);
         break;
      }
   } while (temp != *head);
   if (flag)
   {
      printf("malloc success!\n");
	   printf("The alloc address next searching is: %p.\n", (*head)->m_addr);
   }
   else
   {
      printf("soory,without enough memory space\n");
      print_all_map(*head, *num);
   }
}

/**
 * @*description: insert a map between two empty map
 * @param {map*} anew
 * @param {map*} lower
 * @param {map*} upper
 * @return {*}
 */
void InsertMap(map *anew, map *lower, map *upper)
{
   lower->next = anew;
   anew->prior = lower;
   anew->next = upper;
   upper->prior = anew;
};

/**
 * @*description: remove the map c
 * @param {map} *p
 * @param {map} *c
 * @param {map} *n
 * @return {*}
 */
void remove_map(map *p, map *c, map *n)
{
   p->next = n;
   n->prior = p;
   free(c);
};

/**
 * @*description: find the first map before address.
 * @param {unsigned} size
 * @param {char} *addr
 * @param {map} *
 * @param {int} num
 * @return {*}
 */
map *findLastMap(char *addr, map **head, int num)
{
   map *temp = *head;
   for (int i = 0; i < num; i++)
   {
      if ((temp->m_addr < addr) && temp->next->m_addr > addr)
         return temp;
      else
         temp = temp->next;
   }
   if (num == 1)
      return *head;
   else
   {
      temp = *head;
      map *temp2 = temp;
      for (int i = 0; i < num; i++)
      {
         if (temp2->m_addr > temp->m_addr)
            temp = temp2;
         temp2 = temp2->next;
      }
      return temp;
   }
}

/**
 * @*description: find the end of the map which has the largest address
 * @param {map} *
 * @param {int} num
 * @return {*}
 */
char *findEndMap(map **head, int num)
{
   map *ptr = *head;
   char *end = (ptr->m_addr + ptr->m_size);
   for (size_t i = 0; i < num; i++)
   {
      if ((ptr->m_addr + ptr->m_size) > end)
         end = (ptr->m_addr + ptr->m_size);
      ptr = ptr->next;
   }
   return end;
}

/**
 * @*description: free memory from address
 * @param {unsigned} size
 * @param {char} *addr
 * @param {map} *
 * @return {*}
 */
int lfree(unsigned size, char *addr, map **head, int *num)
{
   printf("************************************************************\n");
   if (addr < BASE)
   {
      printf("Error address,your address can't lower than %d\n", BASE);
      printf("************************************************************\n");
      return 0;
   }
   if (addr + size > BASE + 1000)
   {
      printf("Error address,you can't free memory more than %d from %d\n", BASE + 1000 - addr, addr);
      printf("************************************************************\n");
      return 0;
   }
   if (*num == 0)
   {
      *head = (map *)malloc(sizeof(map));
      if (*head == NULL)
      {
         fprintf(stderr, "error:initail fail!\n");
         exit(-1);
      }
      (*head)->m_addr = BASE;
      (*head)->m_size = size;
      (*head)->next = *head;
      (*head)->prior = *head;
      *num += 1;
      printf("free success!");
      printf("************************************************************\n");
      return 1;
   }
   map *ptr = NULL;                              //辅助指针
   map *lastmap = findLastMap(addr, head, *num); //上一个map
   map *nextmap = lastmap->next;                 //下一个map
   char *endaddress = findEndMap(head, *num);    //地址最高的map的尾地址
   // while ((nextmap->m_addr + nextmap->m_size) <= addr) //保证nextmap的终点在addr前
   // {
   //    ptr = nextmap->next;
   //    lastmap->next = ptr;
   //    remove_map(lastmap, ptr, nextmap);
   // }

   char *lastaddr = addr + size; //释放区的末尾地址

   if (addr <= ((lastmap->m_addr) + lastmap->m_size))
   {
      //情况1和2
      if (lastaddr < (lastmap->m_addr + lastmap->m_size))
      {
         printf("The memory has been freed!\n");
         return 0;
      }
      if (nextmap->m_addr > addr)
      {
         //情况1
         lastmap->m_size = addr + size - (lastmap->m_addr);
      }
      else
      {
         //情况2
         lastmap->m_size = nextmap->m_addr + nextmap->m_size - addr;
         lastmap->next = lastmap->next;
         ptr = lastmap->next;
         remove_map(lastmap, nextmap, ptr);
         *num--;
      }
   }
   else
   {
      if (nextmap->m_addr > addr)
      {
         //情况4
         ptr = (map *)malloc(sizeof(map));
         ptr->m_size = size;
         ptr->m_addr = addr;
         InsertMap(ptr, lastmap, nextmap);
         *num += 1;
      }
      else
      {
         //情况3
         nextmap->m_size = nextmap->m_size + addr - nextmap->m_addr;
         nextmap->m_addr = addr;
      }
   }
   return 1;
}

/**
 * @*description: Processing user input
 * @param {map} *
 * @param {int} *num
 * @return {*}
 */
void linput(map **head, int *num)
{
   int flag = 1;
   while (flag)
   {
      printf("************************************************************\n");
      printf("please choose the operation you want\n");
      printf("tips:\n");
      printf("M/m : malloc\n");
      printf("F/f : free\n");
      printf("l/L  : show all of maps\n");
      printf("q/Q   : quit\n");
      printf("************************************************************\n");
      char ch = '\0';
      char para = '\0';
      scanf("%c", &para);
      while ((ch = getchar()) != EOF && ch != '\n')
         ;
      int size = 0, offset = 0;
      char *ptr = NULL;
      switch (para)
      {
      case 'm':
      case 'M':
         printf("Please input the size to be allocated.\n");
         scanf("%d", &size);
         while ((ch = getchar()) != EOF && ch != '\n')
            ;
         if (size <= 0)
         {
            printf("The size should be positive.\n");
            return;
         }
         lmalloc(size, num, head);
         break;
      case 'f':
      case 'F':
         printf("Please specify the address of memory to be freed.\n");
         printf("NOTE: input the relative address(aka offset, which ranges in [0, 1000]) is OK.\n");
         scanf("%d", &offset);
         while ((ch = getchar()) != EOF && ch != '\n')
            ;
         if (offset < 0)
         {
            printf("The offset should be positive.\n");
            return;
         }
         ptr = (char *)(BASE + offset);
         printf("Please input the size of freed memory.\n");
         printf("NOTE: the size should be a integar.\n");
         scanf("%d", &size);
         while ((ch = getchar()) != EOF && ch != '\n')
            ;
         if (size <= 0)
         {
            printf("The size should be positive.\n");
            return;
         }
         if (lfree(size, ptr, head, num))
         {
            printf("Free succeeded!\n");
            print_all_map(*head, *num);
         }
         else
         {
            printf("Free failed!\nPlease re-enter the size and address!\n");
         }
         break;
      case 'q':
      case 'Q':
         flag = 0;
         break;
      case 'l':
      case 'L':
         print_all_map(*head,*num);
         break;
      default:
         printf("Please enter the correct key word!\n");
      };
   }
};

/**
 * @description: Finish the whole program and exit safely.
 * @param {*}
 * @return {*}
 */
void mexit(map *head,int number)
{
   map *ptr = head;
	for (int i = 0; i < number; i++)
	{
		ptr = head->next;
		free(head);
		printf("Having free the %d th map.\n", i + 1);
		head = ptr;
	}
   free(BASE);
   printf("exit\n");
}

int main(void)
{
   int num = 0;
   map *head = NULL;
   initial(&num, &head);
   linput(&head,&num);
   mexit(head,num);
   system("pause");
   return 0;
}
