/******************************************************************************
	��Ŀ����	��SGE800���������ն�ҵ��ƽ̨
	�ļ�		��list.h
	����		�����ļ�������ͨ���������
	�汾		��0.1
	����		����迡
	��������	��2010.11
******************************************************************************/

#ifndef _LIST_H
#define _LIST_H

#define NULL ((void *) 0)

typedef unsigned int u32;

//����ͷ����
struct list_head {
	struct list_head *next, *prev;
};


/*************************************************
  ����ڵ��ʼ��
*************************************************/
#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}


/*************************************************
  ����ڵ���ӹ���
*************************************************/
//���һ���ڵ�new�����������Ľڵ�prev��next֮��
static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}
//�ڽڵ�head֮�����һ���ڵ�new
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}
//�ڽڵ�head֮ǰ���һ���ڵ�new
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}



/*************************************************
  ����ڵ�ɾ������
*************************************************/
//�������ڵ�prev��next�������ӣ�����������ɾ���������ڵ�֮������нڵ�
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}
//ɾ��һ���ڵ�entry��������δ��ʼ��״̬
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}
//ɾ��һ���ڵ�entry�������³�ʼ��
static inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}



/*************************************************
  ����ڵ��滻����
*************************************************/
//�ýڵ�new�滻�ڵ�old�������޸Ľڵ�old��ֵ
static inline void list_replace(struct list_head *old,
				struct list_head *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}
//�ýڵ�new�滻�ڵ�old�������³�ʼ���ڵ�old
static inline void list_replace_init(struct list_head *old,
					struct list_head *new)
{
	list_replace(old, new);
	INIT_LIST_HEAD(old);
}


/*************************************************
  ����ڵ��ƶ�����
*************************************************/
//�ƶ�һ���ڵ�list��head֮ǰ
static inline void list_move(struct list_head *list, struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add(list, head);
}
//�ƶ�һ���ڵ�list��head֮��
static inline void list_move_tail(struct list_head *list,
				  struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add_tail(list, head);
}




/*************************************************
  ����ڵ��жϹ���
*************************************************/
//�ж�list�ڵ��ǲ���head��������һ��
static inline int list_is_last(const struct list_head *list,
				const struct list_head *head)
{
	return list->next == head;
}
//�ж�head�ڵ��ǲ��ǿ���
static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}
//�̰߳�ȫ�ж�head�ڵ��ǲ��ǿ���
static inline int list_empty_careful(const struct list_head *head)
{
	struct list_head *next = head->next;
	return (next == head) && (next == head->prev);
}
//�ж�head�����ǲ���ֻ����һ���ڵ�
static inline int list_is_singular(const struct list_head *head)
{
	return !list_empty(head) && (head->next == head->prev);
}



/*************************************************
  ����ָ��
*************************************************/
//���ڱ�list_cut_position����
static inline void __list_cut_position(struct list_head *list,
		struct list_head *head, struct list_head *entry)
{
	struct list_head *new_first = entry->next;
	list->next = head->next;
	list->next->prev = list;
	list->prev = entry;
	entry->next = list;
	head->next = new_first;
	new_first->prev = head;
}
//��һ������ֳ���������list���²���������head��ԭ�е�����entry֮��Ĺ�head��֮ǰ������entry����list��
static inline void list_cut_position(struct list_head *list,
		struct list_head *head, struct list_head *entry)
{
	if (list_empty(head))
		return;
	if (list_is_singular(head) &&
		(head->next != entry && head != entry))
		return;
	if (entry == head)
		INIT_LIST_HEAD(list);
	else
		__list_cut_position(list, head, entry);
}



/*************************************************
  ����ϲ�����
*************************************************/
//���ڱ�list_splice����
static inline void __list_splice(const struct list_head *list,
				 struct list_head *prev,
				 struct list_head *next)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}
//��head�ڵ�֮�����list����
static inline void list_splice(const struct list_head *list,
				struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head, head->next);
}
//��head�ڵ�֮ǰ����list����
static inline void list_splice_tail(struct list_head *list,
				struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head->prev, head);
}
//��head�ڵ�֮�����list��������list�������³�ʼ��
static inline void list_splice_init(struct list_head *list,
				    struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head, head->next);
		INIT_LIST_HEAD(list);
	}
}
//��head�ڵ�֮ǰ����list��������list�������³�ʼ��
static inline void list_splice_tail_init(struct list_head *list,
					 struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head->prev, head);
		INIT_LIST_HEAD(list);
	}
}



/*************************************************
  �����������
*************************************************/
#define offsetof(TYPE, MEMBER) ((u32) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

//��ð����ڵ�Ľṹ
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)
//��ô�����ĵ�һ���ڵ�ṹ
#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)
//�����������head�ڵ�
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)
//�����������head�ڵ�
#define list_for_each_reverse(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)
//�����������head���ڵĽṹ
#define list_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))
//�����������head���ڵĽṹ
#define list_for_each_entry_reverse(pos, head, member)			\
	for (pos = list_entry((head)->prev, typeof(*pos), member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.prev, typeof(*pos), member))
//��pos�ṹ��ʼ��������������ڵĽṹ��������pos����
#define list_for_each_entry_continue(pos, head, member) 		\
	for (pos = list_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head);	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))
//��pos�ṹ��ʼ��������������ڵĽṹ��������pos����
#define list_for_each_entry_continue_reverse(pos, head, member)		\
	for (pos = list_entry(pos->member.prev, typeof(*pos), member);	\
	     &pos->member != (head);	\
	     pos = list_entry(pos->member.prev, typeof(*pos), member))
//��pos�ṹ��ʼ��������������ڵĽṹ������pos����
#define list_for_each_entry_from(pos, head, member) 			\
	for (; &pos->member != (head); pos = list_entry(pos->member.next, typeof(*pos), member))


#endif

