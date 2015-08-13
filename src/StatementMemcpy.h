/*
 * StatementMemcpy.h
 *
 *  Created on: 11 Aug 2015
 *      Author: jacek
 */

#ifndef STATEMENTMEMCPY_H_
#define STATEMENTMEMCPY_H_

#include "Statement.h"

class StatementMemcpy: public Statement {
public:
	StatementMemcpy(Block* b, const ExpressionVariable &dst, const ExpressionVariable &src);
	virtual ~StatementMemcpy();
	static Statement *make_random(CGContext &cg_context);
	virtual void get_blocks(std::vector<const Block*>& /* blks */) const {};
	virtual void get_exprs(std::vector<const Expression*>& exps) const {};
	virtual void Output(std::ostream &out, FactMgr* fm, int indent = 0) const;
private:
	const ExpressionVariable &var_src;
	const ExpressionVariable &var_dst;
};

#endif /* STATEMENTMEMCPY_H_ */
