
--------------------------------------------------------------------------------

function cloneTable( essence )
	local instance = {}
	for key, value in pairs( essence ) do
		if "table" == type( value ) then
			instance[ key ] = cloneTable( value )
		else
			instance[ key ] = value
		end
	end	
	return instance
end

--------------------------------------------------------------------------------

function concatTables( source, receiver, replaceFields )
	if nil == receiver then
		receiver = {}
	end
	for key, value in pairs( source ) do
		if replaceFields then
			receiver[ key ] = value
		else
			if nil == receiver[ key ] then
				receiver[ key ] = value	
			end
		end
	end	
	return instance
end

--------------------------------------------------------------------------------

function printValue( value )
	if nil == value then
		DebugOut( "nil" )
	else
		valueType = type( value )
		if "table" == valueType then
			outTableBrief( value )
		elseif "function" == valueType then
			DebugOut( "Function" )
		elseif "boolean" == valueType then
			if value then
				DebugOut( "true" )
			else
				DebugOut( "false" )
			end
		else
			DebugOut( ""..value )
		end
	end
end

--------------------------------------------------------------------------------

function outTable( luaTable, shift )
	if nil == shift then
		shift = ""
		DebugOut( "*******************************************" )
	end
	if nil ~= luaTable then
		for key, value in pairs( luaTable ) do
			fieldType = type( value )
			if "table" == fieldType then
				DebugOut( shift.."   ----------------" )
				DebugOut( shift.."   Table: "..key )
				outTable( value, shift.."   " )	
				DebugOut( shift.."   ----------------" )
			elseif "function" == fieldType then
				DebugOut( shift.."Function: "..key )
			elseif "boolean" == fieldType then
				str = shift.."["..key.."] = ";
				if value then
					str = str.."true"
				else
					str = str.."false"
				end
				DebugOut( str )
			else
				DebugOut( shift.."["..key.."] = "..value )
			end
		end	
	else
		DebugOut( shift.."nil" )
	end
	if "" == shift then
		DebugOut( "*******************************************" )
	end
end

--------------------------------------------------------------------------------

function outTableBrief( luaTable )
	DebugOut( "*******************************************" )
	if nil ~= luaTable then
		for key, value in pairs( luaTable ) do
			fieldType = type( value )
			if "table" == fieldType then
				DebugOut( "Table: "..key )
			elseif "function" == fieldType then
				DebugOut( "Function: "..key )
			elseif "boolean" == fieldType then
				str = "["..key.."] = ";
				if value then
					str = str.."true"
				else
					str = str.."false"
				end
				DebugOut( str )
			else
				DebugOut( "["..key.."] = "..value )
			end
		end	
	else
		DebugOut( "nil" )
	end
	DebugOut( "*******************************************" )
end

--------------------------------------------------------------------------------

function isEqualPoints( point1, point2 )
	return point1.x == point2.x and point1.y == point2.y
end

--------------------------------------------------------------------------------

function sign( val )
	if 0 == val then
		return 0
	elseif 0 < val then
		return 1
	else
		return -1
	end
end

--------------------------------------------------------------------------------
