# Copyright (C) 2015 Ishita Takeshi

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


module PoseEstimator


export Candidate, create_estimator, estimate


type Point
    x::Uint64
    y::Uint64
end


type CCandidate
    size::Uint64
    parts::Ptr{Ptr{Point}}
    confidence::Ptr{Float32}
end


type CCandidates
    size::Uint64
    candidates::Ptr{Ptr{CCandidate}}
end


"""
Pointer handler
We need this type since the finalizer function in Julia
doesn't allow the Ptr{Void} type variable.
"""
type PointerHandler{T}
    pointer::Ptr{T}
end


clib_filename = "pose/lib/libPartsBasedDetector.so"


"""Finalize a PointerHandler type variable by the finalizer"""
function pointer_finalizer(handler::PointerHandler, finalizer::Function)
    if handler.pointer != C_NULL
        finalizer(handler.pointer)
        handler.pointer = C_NULL
    end
end


function create_estimator(model_filename)
    p = ccall((:create_estimator, "pose/lib/libPartsBasedDetector.so"),
              Ptr{Void}, (Cstring,), model_filename)
    estimator = PointerHandler(p)
    finalizer(estimator, x -> pointer_finalizer(x, destroy_estimator))
    return estimator
end


function estimate_(estimator::PointerHandler, image_filename)
    p = ccall((:estimate, "pose/lib/libPartsBasedDetector.so"),
              Ptr{CCandidates}, (Ptr{Void}, Cstring),
              estimator.pointer, image_filename)
    candidates = PointerHandler(p)
    finalizer(candidates, x -> pointer_finalizer(x, free_candidates))
    return candidates
end


function destroy_estimator(estimator)
    ccall((:destroy_estimator, "pose/lib/libPartsBasedDetector.so"),
          Void, (Ptr{Void},), estimator)
end


function free_candidates(candidates)
    ccall((:free_candidates, "pose/lib/libPartsBasedDetector.so"),
          Void, (Ptr{CCandidates},), candidates)
end


type Candidate
    parts
    confidence::Array{Float32}
end


function estimate(estimator::PointerHandler, image_filename)
    function load_parts(candidate)
        array = pointer_to_array(candidate.parts, candidate.size)

        points = Array(Uint64, candidate.size, 2)
        for (i, p) in enumerate(array)
            point = unsafe_load(p)
            points[i, :] = [point.x point.y]
        end

        return points
    end

    function load_confidence(candidate)
        pointer_to_array(candidate.confidence, candidate.size)
    end

    function load_candidate(candidate_pointer)
        candidate = unsafe_load(candidate_pointer)
        parts = load_parts(candidate)
        confidence = load_confidence(candidate)
        Candidate(parts, confidence)
    end

    p = estimate_(estimator, image_filename)
    c = unsafe_load(p.pointer)

    pointer_array = pointer_to_array(c.candidates, c.size)
    return [load_candidate(c) for c in pointer_array]
end

end
