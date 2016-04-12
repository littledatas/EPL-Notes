/*
When x crosses a boundary, it may encroach on y's region. in this case 
Region resize callback
x.border_cross()
	space.update_position(x, ...)
		y.region_resize() 	// insert, remove, or update_position will invoke quadtree, which will automatically call region_resize

update time and position of the object you store are correctly managed because:
	if two organisms are moving toward one another and one crosses a border, it will call update_position
	it's possible in that time, when region_resize is called on y, y has already crossed the new border
	this calls update_position for x again, so you need to make sure the state is already correct when it is called again
		and that the timestep is 0

don't cancel event in bordercross because it has already happened ??
*/