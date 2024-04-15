from flask import Flask, render_template, request

app = Flask(__name__)

class Pagination:
    def __init__(self, total_items, page_size):
        self.total_items = total_items
        self.page_size = page_size
        self.current_page = 1
        self.search_keyword = None
        self.sort_by = None

    def get_total_pages(self):
        return (self.total_items + self.page_size - 1) // self.page_size

    def get_items_for_current_page(self):
        start_index = (self.current_page - 1) * self.page_size
        end_index = min(start_index + self.page_size, self.total_items)
        items = list(range(self.total_items))  # 예시 데이터
        items = self.apply_search_filter(items)
        items = self.apply_sort_filter(items)
        return items[start_index:end_index]

    def go_to_page(self, page_number):
        total_pages = self.get_total_pages()
        if 1 <= page_number <= total_pages:
            self.current_page = page_number
        elif page_number < 1:
            self.current_page = 1
        else:
            self.current_page = total_pages

    def previous_page(self):
        if self.current_page > 1:
            self.current_page -= 1

    def next_page(self):
        total_pages = self.get_total_pages()
        if self.current_page < total_pages:
            self.current_page += 1

    def set_search_keyword(self, keyword):
        self.search_keyword = keyword

    def clear_search_keyword(self):
        self.search_keyword = None

    def apply_search_filter(self, items):
        if self.search_keyword:
            return [item for item in items if self.search_keyword.lower() in str(item).lower()]
        else:
            return items

    def set_sort_by(self, sort_by):
        self.sort_by = sort_by

    def clear_sort_by(self):
        self.sort_by = None

    def apply_sort_filter(self, items):
        if self.sort_by == 'ascending':
            return sorted(items)
        elif self.sort_by == 'descending':
            return sorted(items, reverse=True)
        else:
            return items

pagination = Pagination(total_items=100, page_size=10)

@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        search_keyword = request.form.get('search_keyword', '').strip()
        pagination.set_search_keyword(search_keyword)

        sort_by = request.form.get('sort_by')
        pagination.set_sort_by(sort_by)
    else:
        pagination.clear_search_keyword()
        pagination.clear_sort_by()

    page = int(request.args.get('page', 1))
    pagination.go_to_page(page)

    items = pagination.get_items_for_current_page()

    return render_template('index.html', items=items, pagination=pagination)

if __name__ == '__main__':
    app.run(debug=True)
