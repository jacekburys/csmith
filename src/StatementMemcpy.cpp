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

	Effect running_eff_context(cg_context.get_effect_context());
	Effect src_accum, dst_accum;
	CGContext src_cg_context(cg_context, running_eff_context, &src_accum);

	const Variable* src = NULL;
	const Variable* dst = NULL;
	vector<const Variable*> dummy;

	const Type* type;

	do{
		type = Type::SelectLType(!cg_context.get_effect_context().is_side_effect_free(), eSimpleAssign);
	}while(type->simple_type == eVoid);

	assert(type && "type is null");

	CVQualifiers qfer;
	do{
		qfer = CVQualifiers::random_qualifiers(type);
	}while(qfer.is_const());


	do{
		src = VariableSelector::select(Effect::READ, src_cg_context, type, &qfer, dummy, eExact);
	}while(src == NULL || src->isBitfield_);

	running_eff_context.add_effect(src_accum);

	cg_context.merge_param_context(src_cg_context, true);
	running_eff_context.write_var_set(src_accum.get_lhs_write_vars());

	ExpressionVariable* exp_src = new ExpressionVariable(*src);


	CGContext dst_cg_context(cg_context, running_eff_context, &dst_accum);
	dst_cg_context.get_effect_stm() = src_cg_context.get_effect_stm();
	dst_cg_context.curr_rhs = exp_src;

	/*
	assert(src && "src is null");

	do{
		dst = VariableSelector::select(Effect::WRITE, dst_cg_context, type, &qfer, dummy, eExact);
	}while(dst == NULL || dst->isBitfield_
			|| dst->to_string() == src->to_string() || dst->type->SizeInBytes() != src->type->SizeInBytes());

	assert(dst && "dst is null");
	*/

	do{
		Lhs* lhs = Lhs::make_random(dst_cg_context, type, &qfer, false, true);
		dst = lhs->get_var();
	}while(dst == NULL || dst->isBitfield_
			|| dst->to_string() == src->to_string() || dst->type->SizeInBytes() != src->type->SizeInBytes());


	ExpressionVariable* exp_dst = new ExpressionVariable(*dst);

	cg_context.merge_param_context(dst_cg_context, true);

	assert(dst->type->SizeInBytes() == src->type->SizeInBytes());
	assert(dst->to_string() != src->to_string());



	StatementMemcpy* res = new StatementMemcpy(curr_func->body, *exp_dst, *exp_src);

	//res->Output(cerr, NULL, 0);

	return res;// new StatementMemcpy(curr_func->body, exp_dst, exp_src);
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

	//assert(var_dst != NULL && "var_dst is NULL");

	var_dst.Output(out);



	out << " != &";

	//assert(var_src != NULL && "var_src is NULL");

	var_src.Output(out);
	out << ")" << endl;



	string sizeofstring;
	//var_src.get_type().get_base_type()->get_type_sizeof_string(sizeofstring);
	var_dst.get_type().get_type_sizeof_string(sizeofstring);

	output_tab(out, indent+1);
	out << "memcpy(&";
	var_dst.Output(out);
	out << ", &";
	var_src.Output(out);
	out << ", " << var_dst.get_type().SizeInBytes() << ");";
	//out << var_src.get_type().get_base_type()->SizeInBytes() << ");";
	outputln(out);
}
