// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2010, Knut Reinert, FU Berlin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: David Weese <david.weese@fu-berlin.de>
// ==========================================================================

#ifndef SEQAN_HEADER_PIPE_NAMER_H
#define SEQAN_HEADER_PIPE_NAMER_H

namespace SEQAN_NAMESPACE_MAIN
{

//namespace SEQAN_NAMESPACE_PIPELINING
//{

    template < typename TCompare >
    struct Namer;

	template < typename TInput, typename TCompare >
    struct Value< Pipe< TInput, Namer<TCompare> > >
    {
        typedef Pair<
            typename Value<typename Value<TInput>::Type, 1>::Type,
			typename Size<TInput>::Type,
			Pack
		> Type;
    };


/**
.Spec.Namer:
..cat:Pipelining
..general:Class.Pipe
..summary:Extends the input stream by a second field which names the elements.
..signature:Pipe<TInput, Namer<TCompare> >
..param.TInput:The type of the pipeline module this module reads from.
..param.TCompare:A binary function (see STL's $binary_function$) with result type $int$.
...remarks:Should return $0$ if and only if two elements are equal.
..remarks:The output type is a @Class.Pair@ of input type and size type (i.e. $Pair<Value<TInput>::Type, Size<TInput>::Type>$).
..remarks:The first output field is the original input stream.
..remarks:The second output field is the name. This field begins with 0 and increases by 1 for every distinct element. Two elements gets the same name, if and only if they are equal.
..include:seqan/pipe.h
*/

    //////////////////////////////////////////////////////////////////////////////
    // namer class
    template < typename TInput, typename TCompare >
    struct Pipe< TInput, Namer<TCompare> >
    {
		TInput                          &in;
        TCompare                        C;
        typename Value<Pipe>::Type      tmp;
        typename Value<TInput>::Type    last;

/**
.Memfunc.Namer#Pipe:
..class:Spec.Namer
..summary:Constructor
..signature:Pipe<TInput, Namer<TCompare> > (in)
..signature:Pipe<TInput, Namer<TCompare> > (in, comp)
..param.in:Reference to an input pipe.
..param.comp:A $TCompare$ object (copy constructor).
*/
        Pipe(TInput& _in):
            in(_in) {}
        
        Pipe(TInput& _in, const TCompare& tmpC) :
            in(_in),
            C(tmpC) {}
        
        inline typename Value<Pipe>::Type const & operator*()
        {
            tmp.i1 = getValueI1(*in);
            return tmp;
        }

        inline Pipe& operator++()
        {
            ++in;
            if (!eof(in) && C(last, *in) != 0) {
                #ifdef SEQAN_TEST
                    SEQAN_ASSERT_LT(C(last, *in), 0);
                #endif
                last = *in;
                ++tmp.i2;
            }
			return *this;
        }

        bool unique() const
        {
            return tmp.i2 == (length(in) - 1);
        }
    };
    

    //////////////////////////////////////////////////////////////////////////////
    // global pipe functions
    template < typename TInput, typename TCompare >
	inline bool control(Pipe< TInput, Namer<TCompare> > &me, ControlBeginRead const &command)
    {
        if (!control(me.in, command)) return false;
        if (!eof(me.in))
        {
            me.last = *me.in;
            me.tmp.i1 = me.last.i1;
        }
        me.tmp.i2 = 0;
		return true;
	}
    
//}

}

#endif
