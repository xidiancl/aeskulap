#include "afloatwidget.h"
#include <iostream>

namespace Aeskulap {

std::set<FloatWidget*> FloatWidget::m_widgetlist;

FloatWidget::FloatWidget(Gtk::Widget& parent, int width, int height) :
m_width(width),
m_height(height) {
	m_parent = &parent;

	resize(m_width, m_height);
	set_decorated(false);
	set_keep_above();
	set_skip_taskbar_hint(true);
	set_skip_pager_hint(true);
	set_type_hint(Gdk::WINDOW_TYPE_HINT_UTILITY);
	reparent(parent);
	
	m_widgetlist.insert(this);
}

FloatWidget::~FloatWidget() {
	m_motion_connection.disconnect();
	m_widgetlist.erase(this);
}

void FloatWidget::on_show() {
	m_motion_connection = Glib::signal_timeout().connect(sigc::bind(sigc::mem_fun(*this, &FloatWidget::on_timeout), 1), 50);
	Gtk::Window::on_show();
}

void FloatWidget::on_hide() {
	m_motion_connection.disconnect();
	Gtk::Window::on_hide();
}
	

void FloatWidget::on_realize() {
	Gtk::Window::on_realize();
	get_window()->reparent(m_parent->get_window(), 0, 0);
}

bool FloatWidget::on_timeout(int timer) {
	int x, x1;
	int y, y1;
	
	m_parent->get_pointer(x, y);
	if(x < 0 || y < 0) {
		return true;
	}

	if(x >= m_parent->get_width() || y >= m_parent->get_height()) {
		return true;
	}

	if(!m_parent->is_visible() || !m_parent->is_realized()) {
		return true;
	}
		
	m_parent->get_pointer(x, y);
	//get_position(x, y);
	/*get_pointer(x1, y1);

	x += x1;
	y += y1;*/

	x += 16;
	y += 16;

	if(x != last_x || y != last_y) {
		move(x, y);
		
		last_x = x;
		last_y = y;
	}

	return true;
}

void FloatWidget::raise_global() {
	std::set<FloatWidget*>::iterator i;
	for(i = m_widgetlist.begin(); i != m_widgetlist.end(); i++) {
		if((*i)->is_visible()) {
			(*i)->raise();
		}
	}
}

} // namespace Aeskulap
