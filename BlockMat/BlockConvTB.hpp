

template <int N, int E, int M>
int bmf_to_bfp_tb(){    // Test BMP to BFP conversion with random stimulus.

    BlockMF<N,E,M>                     bmf_in;    // BMF input.
    BlockFP<N,WPRD(E,M)/2,FPRD(E,M)/2> bfp_out;   // BFP output.

    // Log parameters.
    std::cout << "[INFO] BMF to BFP, Parameters: N=" << N << " E=" << E << " M=" << M << '\n';

    for(int l=0; l<(1<<(E+M+1)); l++){    // Test with random inputs.

        // Generate input randomly.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                bmf_in.data[j][k].data = rand() % (1<<(E+M+1));
            }
        }
        do{
            bmf_in.bias = rand() % (1<<8);
        }while(((bmf_in.bias+WPRD(E,M))>=128) || ((bmf_in.bias-WPRD(E,M))<-128));

        // Get result from DUT.
        bfp_out = bmf_in;

        // Compare DUT result to float32 reference.
        for(int j=0; j<N; j++){
            for(int k=0; k<N; k++){
                if((double(bmf_in.data[j][k]) * pow(2,bmf_in.bias)) != (double(bfp_out.data[j][k]) * pow(2,bfp_out.bias))){

                    // std::cout << "Bias (in, out): " << bmf_in.bias << ", " << bfp_out.bias << '\n';
                    // std::cout << "MF (in, out): " << double(bmf_in.data[j][k]) << ", " << double(bfp_out.data[j][k]) << '\n';
                    // std::cout << "Raw (in, out): " << bmf_in.data[j][k].data << ", " << bfp_out.data[j][k].acc << '\n';
                    // std::cout << WPRD(E,M)/2 << '\n';
                    // std::cout << FPRD(E,M)/2 << '\n';

                    printf("[ERROR] FAILED\n");
                    throw 1;
                }
            }
        }
    }
    printf("[INFO] Pass\n");
    return 0;
}

