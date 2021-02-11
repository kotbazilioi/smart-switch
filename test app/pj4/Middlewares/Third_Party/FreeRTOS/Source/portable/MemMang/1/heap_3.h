#ifndef __HEAP_Z_H
#define __HEAP_Z_H

#pragma pack( push, 1 )
typedef union type_size {
	unsigned long mark;
	struct {
	   unsigned size:24;
	   unsigned type:8;
	}bits;
}type_size;

// Memory Control Block (MCB)
//----------------------------------------------------------------------------
typedef struct heap_mcb {
	struct heap_mcb *next;   	// ��������� �� ��������� MCB. mcb.next  ���������� MCM ������ ��������� ��  ������ MCB.
	struct heap_mcb *prev;   	// ��������� �� ���������� MCB. ��� ������� MCB ���� ��������� ��������� ��� �� ����.
	union type_size ts;  		// ������ ����� ������  �������������� ���� ������ ���������� ����� �� MCB
        unsigned char* owner;                    // ��������� �� ���� ������. ������������ ��������� ��������� ������ � �� �������� ��� ����������� ������
} heap_mcb;


// ��������� ���� 
//----------------------------------------------------------------------------
typedef struct heap_t { 	
	struct heap_mcb *start; // ��������� �� ������ heap (������ MCB)	
	struct heap_mcb *freem; // ��������� �� ������ ��������� MCB	
	unsigned long hsize; // ������ ������ ����
        long work_size; // ������� ����� ����
        long blocks_count; // ����� ����� ������
        long free_blocks_count; // ����� ��������� ������
} heap_t;
#pragma pack( pop )

void init_system_heap( void ); // ������������� 
void heapinit( heap_t *heap ); // ������������� ������������ ������ � �������� heap

// ����������� ������ � ���� 'heap', �������� 'size' ����.
// ���������� ��������� �� ����������������� ������
// ���� � ���� �� ���������� ������, �� ���������� NULL
void *malloc_z( heap_t *heap, size_t size, int type );

// ����������� ����������������� ������, �� ������� ��������� 'ptr' � 'heap'.
// ���� ��������� ��������� �� ������, ������� �� ���� �������� �����
// malloc_z() ��� ��������� 'ptr' ����� 0, �� ������ �� ����������
void free_z( heap_t *heap, void *ptr );

//----------------------------------------------------------------------------
enum MCB_MARK {
	MARK_FREE = 0,
	MARK_SYSTEM = 1
};

// �������� ����� ��������� �����
//----------------------------------------------------------------------------
unsigned long getHeapFreeSize();

// �������� ������������ ������ ���������� �����
//----------------------------------------------------------------------------
unsigned long getHeapMaxSize( heap_t *heap);
unsigned long getHeapSize();

void mergeBlocks( heap_t *heap, heap_mcb* tptr, heap_mcb* xptr );
void shiftBlocks( heap_t *heap, heap_mcb* tptr, heap_mcb* fptr );
void defragHeap( heap_t *heap);

// ��������� � ���� ��� ���� ���� ������
void heapadd( heap_t *heap, heap_mcb *addr, int size );
#endif /* __HEAP_Z_H */