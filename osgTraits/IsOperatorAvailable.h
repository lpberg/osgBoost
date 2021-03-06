/** @file
	@brief Header

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_IsOperatorAvailable_h_GUID_ea6e8ae3_7a30_4c88_b99e_5f12b40ee59b
#define INCLUDED_IsOperatorAvailable_h_GUID_ea6e8ae3_7a30_4c88_b99e_5f12b40ee59b

// Internal Includes
#include "InvokeOperator.h"
#include "Operators.h"
#include "OperatorArity.h"
#include "OperationArguments.h"
#include "ConstructOperation.h"
#include "ConstructBoundOperation.h"
#include "AddArgType.h"

// Library/third-party includes
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/list/list10.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/lambda.hpp>


// Standard includes
// - none

namespace osgTraits {

	namespace availability_detail {
		namespace mpl = boost::mpl;
		using namespace boost::mpl::placeholders;
		using boost::is_base_and_derived;
		using boost::enable_if;

		template<typename Operation>
		struct is_operation_available : mpl::not_<is_base_and_derived<detail::UnimplementedOperationBase, get_operation_invoker<Operation> > > {};

		template<typename Operation, typename T>
		struct is_bound_operation_available : is_operation_available< add_argtype<Operation, T> > {};

		typedef mpl::back_inserter< mpl::list0<> > inserter_type;
		template<typename Operation>
		struct get_valid_other_arg_types :
				mpl::copy_if <
				other_argument_types,
				typename mpl::lambda<is_bound_operation_available<Operation, _> >::type,
				inserter_type
				> {};

		template<typename Operator, typename T, typename = void>
		struct is_operator_applicable;

		template<typename Operator, typename T>
		struct is_operator_applicable < Operator, T,
				typename enable_if<is_operator_unary<Operator> >::type >
				: is_operation_available<typename mpl::apply<construct_operation<Operator, T> >::type> {};

		template<typename Operation>
		struct bound_operation_has_implementations {
			typedef typename mpl::not_<mpl::empty<typename get_valid_other_arg_types<Operation>::type > >::type type;
		};

		template<typename Operator, typename T>
		struct is_operator_applicable<Operator, T, typename enable_if<is_operator_binary<Operator> >::type > : mpl::or_ <
				bound_operation_has_implementations<construct_bound_operation<Operator, T, 0> >,
				bound_operation_has_implementations<construct_bound_operation<Operator, T, 1> > > {};

	} // end of namespace availability_detail

	using availability_detail::get_valid_other_arg_types;
	using availability_detail::is_operation_available;
	using availability_detail::is_operator_applicable;

} // end of namespace osgTraits

#endif // INCLUDED_IsOperatorAvailable_h_GUID_ea6e8ae3_7a30_4c88_b99e_5f12b40ee59b
