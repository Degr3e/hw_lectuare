#include <iostream>
#include <cstddef>
#include <stdexcept>

template< class T >
struct BiList
{
  T val;
  BiList< T >* next;
  BiList< T >* prev;
};

// ЗАДАЧА № 1
// Неявный интерфейс для T:
// - Конструктор копирования
// - Оператор копирующего присваивания
// - Деструктор

// ЗАДАЧА № 2

template< class T >
BiList< T >* create(const T& value);

template< class T >
BiList< T >* add(BiList< T >* h, const T& value);

template< class T >
BiList< T >* insert(BiList< T >* h, const T& value);

template< class T >
BiList< T >* cut(BiList< T >* h) noexcept;

template< class T >
BiList< T >* erase(BiList< T >* h) noexcept;

template< class T >
BiList< T >* clear(BiList< T >* h) noexcept;

template< class T >
BiList< T >* fake(BiList< T >* h);

template< class T >
BiList< T >* rmfake(BiList< T >* h);

template< class T, class F >
F traverse(F f, BiList< T >* h, BiList< T >* e);

template< class T, class F >
F traverse_back(F f, BiList< T >* h, BiList< T >* e);

// ЗАДАЧА № 3

template< class T >
BiList< T >* create(const T& value)
{
  BiList< T >* node = new BiList< T >{value, nullptr, nullptr};
  node->next = node;
  node->prev = node;
  return node;
}

template< class T >
BiList< T >* add(BiList< T >* h, const T& value)
{
  BiList< T >* newNode = new BiList< T >{value, h, h->prev};
  h->prev->next = newNode;
  h->prev = newNode;
  return newNode;
}

template< class T >
BiList< T >* insert(BiList< T >* h, const T& value)
{
  return add(h->next, value);
}

template< class T >
BiList< T >* cut(BiList< T >* h) noexcept
{
  BiList< T >* ret = h->next;
  h->prev->next = ret;
  ret->prev = h->prev;
  delete h;
  return ret;
}

template< class T >
BiList< T >* erase(BiList< T >* h) noexcept
{
  return cut(h->next);
}

template< class T >
BiList< T >* clear(BiList< T >* h) noexcept
{
  BiList< T >* current = h->next;
  while (current != h)
  {
    BiList< T >* next = current->next;
    delete current;
    current = next;
  }
  h->next = h;
  h->prev = h;
  return h;
}

template< class T >
BiList< T >* fake(BiList< T >* h)
{
  BiList< T >* f = static_cast< BiList< T >* >(::operator new(sizeof(BiList< T >)));
  f->next = h;
  f->prev = h->prev;
  h->prev->next = f;
  h->prev = f;
  return f;
}

template< class T >
BiList< T >* rmfake(BiList< T >* h)
{
  BiList< T >* ret = h->next;
  h->prev->next = ret;
  ret->prev = h->prev;
  ::operator delete(h);
  return ret;
}

template< class T, class F >
F traverse(F f, BiList< T >* h, BiList< T >* e)
{
  BiList< T >* curr = h;
  while (true)
  {
    f(curr->val);
    if (curr == e) break;
    curr = curr->next;
  }
  return f;
}

template< class T, class F >
F traverse_back(F f, BiList< T >* h, BiList< T >* e)
{
  BiList< T >* curr = h;
  while (true)
  {
    f(curr->val);
    if (curr == e) break;
    curr = curr->prev;
  }
  return f;
}

template< class T >
BiList< T >* find_last(BiList< T >* h)
{
  return h->prev;
}

// ЗАДАЧА № 4

template< class T >
BiList< T >* convert(const T* a, size_t size)
{
  if (size == 0) return nullptr;

  BiList< T >* head = nullptr;
  try
  {
    head = create(a[0]);
    for (size_t i = 1; i < size; ++i)
    {
      add(head, a[i]);
    }
  }
  catch (...)
  {
    if (head)
    {
      BiList< T >* current = head->next;
      while (current != head)
      {
        BiList< T >* next = current->next;
        delete current;
        current = next;
      }
      delete head;
    }
    throw;
  }
  return fake(head);
}

// Демонстрационная программа

int main()
{
  size_t size = 5;
  int* arr = new int[size]{10, 20, 30, 40, 50};

  BiList< int >* list = nullptr;
  try
  {
    list = convert(arr, size);

    auto print = [](int x) { std::cout << x << " "; };

    std::cout << "Прямой порядок: ";
    traverse(print, list->next, find_last(list));
    std::cout << '\n';

    insert(list->next->next, 99);
    std::cout << "После вставки 99 после второго: ";
    traverse(print, list->next, find_last(list));
    std::cout << '\n';

    std::cout << "Обратный порядок: ";
    traverse_back(print, find_last(list), list->next);
    std::cout << '\n';

    add(list->next->next, 88);
    std::cout << "После вставки 88 перед вторым: ";
    traverse(print, list->next, find_last(list));
    std::cout << '\n';

    erase(list->next->next);
    std::cout << "После удаления третьего: ";
    traverse(print, list->next, find_last(list));
    std::cout << '\n';

    BiList< int >* head = rmfake(list);
    clear(head);
    delete head;
    list = nullptr;

    std::cout << "Список очищен.\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
  }
  catch (...)
  {
    std::cerr << "Unknown error\n";
  }

  delete[] arr;
  return 0;
}

