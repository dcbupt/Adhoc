#include "list.h"
//#include "types.h"
#include "includes.h"

//初始化列表
void init_list(list *mlist) {
	mlist->m_size = 0;
}


//列表添加元素
INT8U add_elem(list *mlist, INT8U elem) {
	if (mlist->m_size == MAX_LIST_SIZE) {
		return LIST_OP_FAILED;
	} 

	mlist->m_list[mlist->m_size++] = elem;

	return LIST_OP_SUCCESS;
}

//列表删除元素
void remove_elem(list *mlist, INT8U elem){
	INT8U idx;
	INT8U size;
	if (mlist->m_size == 0) {
		return;
	}
	
	idx = 0;
	size = mlist->m_size;
	for (; idx < size; ++idx) {
		if (mlist->m_list[idx] == elem) {
			for (; idx < size - 1; ++idx) {
				mlist->m_list[idx] = mlist->m_list[idx + 1];
			}

			mlist->m_size--;
			break;
		}
	}

}

//根据元素下标获得元素值
INT8U get_idx_of_elem(list *mlist, INT8U index) {
	if (index < mlist->m_size && index >= 0) {
		return mlist->m_list[index];
	}

	return -1;
}

//判断列表是否为空
INT8U is_list_empty(list *mlist) {
	if (mlist->m_size == 0) {
		return LIST_IS_EMPTY;
	} 

	return LIST_NOT_EMPTY;
}

//返回列表的当前大小
INT8U list_size(list *mlist) {
	return mlist->m_size;
}
