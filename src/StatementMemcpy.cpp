/*
 * StatementMemcpy.cpp
 *
 *  Created on: 11 Aug 2015
 *      Author: jacek
 */

#include "StatementMemcpy.h"
#include <cassert>
#include "Common.h"
#include "Block.h"
#include "CGContext.h"
#include "CGOptions.h"
#include "Constant.h"
#include "ExpressionFuncall.h"
#include "ExpressionVariable.h"
#include "Function.h"
#include "FunctionInvocation.h"
#include "FunctionInvocationBinary.h"
#include "VariableSelector.h"
#include "FactMgr.h"
#include "Lhs.h"
#include "SafeOpFlags.h"
#include "Error.h"
#include "PartialExpander.h"
#include "Bookkeeper.h"
#include "DepthSpec.h"
#include "StatementBreak.h"
#include "StatementFor.h"
#include "CFGEdge.h"
#include "ArrayVariable.h"
#include "random.h"

StatementMemcpy::StatementMemcpy(Block *b, const ExpressionVariable &dst, const ExpressionVariable &src)
  : Statement(eMemcpy, b),
    var_src(src),
    var_dst(dst)
{
	// TODO Auto-generated constructor stub

}

StatementMemcpy::~StatementMemcpy() {
	// TODO Auto-generated destructor stub
}

Statement*
StatementMemcpy::make_random(CGContext &cg_context)
{
	Function *curr_func = cg_context.get_current_func();
	assert(curr_func);
	FactMgr* fm = get_fact_mgr(&cg_context);
	assert(fm);

	const Type* type;

	/*
	//choose type : this is copied from choose_random_pointer_type
	//              except it won't choose pointers to pointers
	const Type* t;
	do{
		t = Type::choose_random();
	} while(t->get_base_type()->eType == eStruct || t->get_base_type()->eType == eUnion);
	ERROR_GUARD(NULL);
	if (t->eType == eSimple) {
		t = get_int_type();
		ERROR_GUARD(NULL);
	}

	type = Type::find_pointer_type(t, true);
	//
	 */

	do{
		type = Type::choose_random_simple();
	}while(type->simple_type == eVoid);


	CVQualifiers qf;
	qf.wildcard = true;

	ExpressionVariable* dst;
	while(true){
		dst = ExpressionVariable::make_random(cg_context, type, &qf, true, false);
		if(!cg_context.is_nonwritable(dst->get_var()) && !dst->get_var()->isBitfield_){
			break;
		}
		delete dst;
	}

	cg_context.write_var(dst->get_var());


	ExpressionVariable* src;
	while(true){
		src = ExpressionVariable::make_random(cg_context, type, &qf, true, false);
		if(!cg_context.is_nonreadable(src->get_var()) && !src->get_var()->isBitfield_ &&
			dst->to_string() != src->to_string() && dst->get_type().SizeInBytes() == src->get_type().SizeInBytes()){
			break;
		}
		delete src;
	}


	cg_context.read_var(src->get_var());


	assert(dst->get_type().SizeInBytes() == src->get_type().SizeInBytes());
	assert(dst->to_string() != src->to_string());


	return new StatementMemcpy(cg_context.get_current_block(), *dst, *src);
}

void
StatementMemcpy::Output(std::ostream &out, FactMgr* /*fm*/, int indent) const
{
	output_tab(out, indent);
	out << "/*MEMCPY*/" << endl;

	output_tab(out, indent);
	out << "if(&";
	/*
	var_dst.Output(out);
	out << " != NULL && ";
	var_src.Output(out);
	out << " != NULL && ";
	*/
	var_dst.Output(out);
	out << " != &";
	var_src.Output(out);
	out << ")" << endl;

	string sizeofstring;
	var_src.get_type().get_base_type()->get_type_sizeof_string(sizeofstring);

	output_tab(out, indent+1);
	out << "memcpy(&";
	var_dst.Output(out);
	out << ", &";
	var_src.Output(out);
	out << ", " << sizeofstring << ");";
	//out << var_src.get_type().get_base_type()->SizeInBytes() << ");";
	outputln(out);
}
