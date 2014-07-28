    public ${entity.className} findById(int id){
    	java.util.List<${entity.className}> list = queryRaw(
				" where " + Properties.Id.columnName
						+ "=" + id);
    	if (list.size()>0){
    		return list.get(0);
    	}
    	return null;
    }
